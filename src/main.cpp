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

const float to_radians = 3.14159265f / 180.0f;

Window main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;

// Vertex shader
static const char* v_shader = "src/Shaders/shader.vert";

// Fragment Shader
static const char* f_shader = "src/Shaders/shader.frag";

void CreateObjects() {

  unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };

  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f, 
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };

  Mesh* obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 12, 12);
  mesh_list.push_back(obj1);

  Mesh* obj2 = new Mesh();
  obj2->CreateMesh(vertices, indices, 12, 12);
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

  GLuint uniform_projection = 0, uniform_model = 0;
  glm::mat4 projection = glm::perspective(45.0f, main_window.GetBufferWidth() / main_window.GetBufferHeight(), 0.1f, 100.0f);

  // Loop until window closed
  while (!main_window.GetShouldClose()) {
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indents indicate levels of drawing and clearing buffers at the end
    shader_list[0].UseShader();
      uniform_model = shader_list[0].GetModelLocation();
      uniform_projection = shader_list[0].GetProjectionLocation();

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
      mesh_list[0]->RenderMesh();

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
      mesh_list[1]->RenderMesh();

    glUseProgram(0);

    main_window.SwapBuffers();
  }

  return 0;
}