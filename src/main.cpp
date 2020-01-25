#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "headers/Mesh.h"
#include "headers/Shader.h"
#include "headers/Window.h"
#include "headers/Camera.h"
#include "headers/Texture.h"
#include "headers/Light.h"

const float to_radians = 3.14159265f / 180.0f;

Window main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;
Camera camera;

Texture brick_texture;
Texture dirt_texture;

Light main_light;

GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

// Vertex shader
static const char* v_shader = "src/Shaders/shader.vert";

// Fragment Shader
static const char* f_shader = "src/Shaders/shader.frag";

void CalcAverageNormals(unsigned int *indices, unsigned int index_count, GLfloat *vertices, 
                        unsigned int vertex_count, unsigned int v_length, unsigned int normal_offset) {
    // Set up the normal light data
    for (size_t i = 0; i < index_count; i += 3) {
      unsigned int in0 = indices[i] * v_length;
      unsigned int in1 = indices[i+1] * v_length;
      unsigned int in2 = indices[i+2] * v_length;
      glm::vec3 v1 (vertices[in1] - vertices[in0], 
                    vertices[in1+1] - vertices[in0+1], 
                    vertices[in1+2] - vertices[in0+2]
      );
      glm::vec3 v2 (vertices[in2] - vertices[in0], 
                    vertices[in2+1] - vertices[in0+1], 
                    vertices[in2+2] - vertices[in0+2]
      );

      glm::vec3 normal = glm::cross(v1, v2);
      normal = glm::normalize(normal); // Transform normal vector to unit vector by normalizing

      in0 += normal_offset;
      in1 += normal_offset;
      in2 += normal_offset;
      vertices[in0] += normal.x; vertices[in0+1] += normal.y; vertices[in0+2] += normal.z;
      vertices[in1] += normal.x; vertices[in1+1] += normal.y; vertices[in1+2] += normal.z;
      vertices[in2] += normal.x; vertices[in2+1] += normal.y; vertices[in2+2] += normal.z;
    }

    for (size_t i = 0; i < vertex_count/v_length; i++) {
      unsigned int n_row_offset = i * v_length + normal_offset;
      glm::vec3 vec(vertices[n_row_offset], vertices[n_row_offset+1], vertices[n_row_offset+2]);
      vec = glm::normalize(vec);
      vertices[n_row_offset] = vec.x; vertices[n_row_offset+1] = vec.y; vertices[n_row_offset+2] = vec.z;
    }
}

void CreateObjects() {

  unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };

  GLfloat vertices[] = {
  // x       y      z       u      v       nx    ny    nz
    -1.0f,  -1.0f,  0.0f,   0.0f,  0.0f,   0.0f, 0.0f, 0.0f, 
     0.0f,  -1.0f,  1.0f,   0.5f,  0.0f,   0.0f, 0.0f, 0.0f, 
     1.0f,  -1.0f,  0.0f,   1.0f,  0.0f,   0.0f, 0.0f, 0.0f, 
     0.0f,   1.0f,  0.0f,   0.5f,  1.0f,   0.0f, 0.0f, 0.0f
  };

  CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

  Mesh* obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 32, 12);
  mesh_list.push_back(obj1);

  Mesh* obj2 = new Mesh();
  obj2->CreateMesh(vertices, indices, 32, 12);
  mesh_list.push_back(obj2);
}


void CreateShaders() {
  Shader *shader1 = new Shader();
  shader1->CreateFromFiles(v_shader, f_shader);
  shader_list.push_back(*shader1);
}


int main() {
  // Create the window, create objects, then compile shaders
  main_window = Window(800, 600);
  main_window.Initialize();

  CreateObjects();
  CreateShaders();

  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

  brick_texture = Texture("src/images/brick.png");
  brick_texture.LoadTexture();
  dirt_texture = Texture("src/images/dirt.png");
  dirt_texture.LoadTexture();

  // Set the ambient light color, intensity, direction and diffuse intensity
  main_light = Light(1.0f, 1.0f, 1.0f, 0.2f, 
                    2.0f, -1.0f, -2.0f, 1.0f);  

  GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0,
        uniform_ambient_intensity = 0, uniform_ambient_color = 0,
        uniform_direction = 0, uniform_diffuse_intensity = 0;
  glm::mat4 projection = glm::perspective(45.0f, main_window.GetBufferWidth() / main_window.GetBufferHeight(), 0.1f, 100.0f);

  // Loop until window closed
  while (!main_window.GetShouldClose()) {
    // Implement delta time functionality
    GLfloat now = glfwGetTime();
    delta_time = now - last_time;
    last_time = now;

    // Get + Handle user input events
    glfwPollEvents();

    camera.KeyControl(main_window.GetKeys(), delta_time);
    camera.MouseControl(main_window.GetXChange(), main_window.GetYChange());

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indents indicate levels of drawing and clearing buffers at the end
    shader_list[0].UseShader();
      uniform_model = shader_list[0].GetModelLocation();
      uniform_projection = shader_list[0].GetProjectionLocation();
      uniform_view = shader_list[0].GetViewLocation();
      uniform_ambient_color = shader_list[0].GetAmbientColorLocation();
      uniform_ambient_intensity = shader_list[0].GetAmbientIntensityLocation();
      uniform_direction = shader_list[0].GetDirectionLocation();
      uniform_diffuse_intensity = shader_list[0].GetDiffuseIntensityLocation();

      main_light.UseLight(uniform_ambient_intensity, uniform_ambient_color,
                          uniform_diffuse_intensity, uniform_direction);

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
      glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.CalcViewMatrix()));
      brick_texture.UseTexture();
      mesh_list[0]->RenderMesh();

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
      dirt_texture.UseTexture();
      mesh_list[1]->RenderMesh();

    glUseProgram(0);

    main_window.SwapBuffers();
  }

  return 0;
}