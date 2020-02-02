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

#include "headers/CommonValues.h"

#include "headers/Mesh.h"
#include "headers/Shader.h"
#include "headers/Window.h"
#include "headers/Camera.h"
#include "headers/Texture.h"
#include "headers/DirectionalLight.h"
#include "headers/PointLight.h"
#include "headers/SpotLight.h"
#include "headers/Material.h"
// #include "headers/Model.h"

#include "headers/Skybox.h"

const float to_radians = 3.14159265f / 180.0f;

GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0,
      uniform_eye_position = 0, uniform_specular_intensity = 0, uniform_shininess = 0,
      uniform_omni_light_pos = 0, uniform_far_plane = 0;

Window main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;
Shader directional_shadow_shader;
Shader omni_shadow_shader;

Camera camera;

Texture brick_texture;
Texture dirt_texture;
Texture plain_texture;

Material shiny_material;
Material dull_material;

// Model xwing;

DirectionalLight main_light;
PointLight point_lights[MAX_POINT_LIGHTS];
SpotLight spot_lights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int point_light_count = 0;
unsigned int spot_light_count = 0;

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
    -1.0f,  -1.0f,  -0.6f,   0.0f,  0.0f,   0.0f, 0.0f, 0.0f, 
     0.0f,  -1.0f,  1.0f,   0.5f,  0.0f,   0.0f, 0.0f, 0.0f, 
     1.0f,  -1.0f, -0.6f,   1.0f,  0.0f,   0.0f, 0.0f, 0.0f, 
     0.0f,   1.0f,  0.0f,   0.5f,  1.0f,   0.0f, 0.0f, 0.0f
  };

  unsigned int floor_indices[] = {
    0, 2, 1,
    1, 2, 3
  };

  GLfloat floor_vertices[] = {
    -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
    10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
  };

  CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

  // Define two triangle meshes
  Mesh* obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 32, 12);
  mesh_list.push_back(obj1);

  Mesh* obj2 = new Mesh();
  obj2->CreateMesh(vertices, indices, 32, 12);
  mesh_list.push_back(obj2);

  // Define the floor mesh
  Mesh *obj3 = new Mesh();
  obj3->CreateMesh(floor_vertices, floor_indices, 32, 6);
  mesh_list.push_back(obj3);
}


void CreateShaders() {
  Shader *shader1 = new Shader();
  shader1->CreateFromFiles(v_shader, f_shader);
  shader_list.push_back(*shader1);

  directional_shadow_shader.CreateFromFiles("src/Shaders/directional_shadow_map.vert", "src/Shaders/directional_shadow_map.frag");
  omni_shadow_shader.CreateFromFiles(
    "src/Shaders/omni_shadow_map.vert", 
    "src/Shaders/omni_shadow_map.geom",
    "src/Shaders/omni_shadow_map.frag"
  );

}

void RenderScene() {
  // Render all the Meshs, Textures and Lights

  glm::mat4 model(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
  brick_texture.UseTexture();
  shiny_material.UseMaterial(uniform_specular_intensity, uniform_shininess);
  mesh_list[0]->RenderMesh(); // Calls the rendering pipeline (vertex -> fragment shaders)

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
  dirt_texture.UseTexture();
  dull_material.UseMaterial(uniform_specular_intensity, uniform_shininess);
  mesh_list[1]->RenderMesh(); 

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
  glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
  dirt_texture.UseTexture();
  shiny_material.UseMaterial(uniform_specular_intensity, uniform_shininess);
  mesh_list[2]->RenderMesh(); 

  // model = glm::mat4(1.0f);
  // model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 10.0f));
  // model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
  // glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
  // shiny_material.UseMaterial(uniform_specular_intensity, uniform_shininess);
  // xwing.RenderModel();

}

void DirectionalShadowMapPass(DirectionalLight *light) {
  // Call the directional shadow map
  directional_shadow_shader.UseShader();
  glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

  light->GetShadowMap()->Write();
  glClear(GL_DEPTH_BUFFER_BIT);

  uniform_model = directional_shadow_shader.GetModelLocation();
  glm::mat4 light_transform = light->CalculateLightTransform();
  directional_shadow_shader.SetDirectionalLightTransform(&light_transform);

  directional_shadow_shader.Validate();

  RenderScene();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OmniShadowMapPass(PointLight *light) {
  omni_shadow_shader.UseShader();
  glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

  light->GetShadowMap()->Write();
  glClear(GL_DEPTH_BUFFER_BIT);

  uniform_model = omni_shadow_shader.GetModelLocation();
  uniform_omni_light_pos = omni_shadow_shader.GetOmniLightPosLocation();
  uniform_far_plane = omni_shadow_shader.GetFarPlaneLocation();

  glUniform3f(uniform_omni_light_pos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
  glUniform1f(uniform_far_plane, light->GetFarPlane());
  omni_shadow_shader.SetLightMatrices(light->CalculateLightTransform());

  omni_shadow_shader.Validate();

  RenderScene();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderPass(glm::mat4 projection_matrix, glm::mat4 view_matrix) {

  glViewport(0, 0, 1366, 768);

  // Clear window
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Then start drawing scene after clearing the screen here.

  skybox.DrawSkybox(view_matrix, projection_matrix);


  // A single pass of rendering meshes
  shader_list[0].UseShader();

  uniform_model = shader_list[0].GetModelLocation();
  uniform_projection = shader_list[0].GetProjectionLocation();
  uniform_view = shader_list[0].GetViewLocation();
  uniform_eye_position = shader_list[0].GetEyePositionLocation();
  uniform_specular_intensity = shader_list[0].GetSpecularIntensityLocation();
  uniform_shininess = shader_list[0].GetShininessLocation();

  // Set the projection and view for the camera
  glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
  glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_matrix));
  // Attached camera position to uniform eye position in the fragment shader
  glUniform3f(uniform_eye_position, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

  shader_list[0].SetDirectionalLight(&main_light);
  shader_list[0].SetPointLights(point_lights, point_light_count, 3, 0);
  shader_list[0].SetSpotLights(spot_lights, spot_light_count, 3 + point_light_count, point_light_count);

  glm::mat4 light_transform = main_light.CalculateLightTransform();
  shader_list[0].SetDirectionalLightTransform(&light_transform);

  main_light.GetShadowMap()->Read(GL_TEXTURE2);
  shader_list[0].SetTexture(1);
  shader_list[0].SetDirectionalShadowMap(2);

  glm::vec3 lower_light = camera.GetCameraPosition();
  lower_light.y -= 0.3f; // Lower the position of the spot light to simulate holding a flash light.
  spot_lights[0].SetFlash(lower_light, camera.GetCameraDirection());

  shader_list[0].Validate();

  RenderScene();
}

int main() {
  // Create the window, create objects, then compile shaders
  main_window = Window(1366, 768);
  main_window.Initialize();

  CreateObjects();
  CreateShaders();

  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

  brick_texture = Texture("src/Textures/brick.png");
  brick_texture.LoadTextureA();
  dirt_texture = Texture("src/Textures/dirt.png");
  dirt_texture.LoadTextureA();
  plain_texture = Texture("src/Textures/plain.png");
  plain_texture.LoadTextureA();

  shiny_material = Material(4.0f, 156);
  dull_material = Material(0.3f, 4);

  // xwing = Model();
  // xwing.LoadModel("Models/x-wing.obj");

  // Set the ambient light color, intensity, direction and diffuse intensity
  main_light = DirectionalLight(2048, 2048,
                                1.0f, 1.0f, 1.0f, 
                                0.1f, 0.8f, 
                                0.0f, -15.0f, -10.0f);  

  // Define the Point Lights
  point_lights[0] = PointLight(1024, 1024, 
                              0.01f, 100.0f,
                              0.0f, 0.0f, 1.0f, 
                              0.0f, 1.0f, 
                              1.0f, 2.0f, 0.0f,
                              0.3f, 0.1f, 0.1f);
  // point_light_count++;
  point_lights[1] = PointLight(1024, 1024, 
                              0.01f, 100.0f,
                              0.0f, 1.0f, 0.0f, 
                              0.0f, 1.0f, 
                              -4.0f, 3.0f, 0.0f,
                              0.3f, 0.1f, 0.1f);
  // point_light_count++;

  // Define the Spot Lights
  spot_lights[0] = SpotLight(1024, 1024, 
                              0.01f, 100.0f,
                              1.0f, 1.0f, 1.0f, 
                              0.0f, 2.0f, 
                              -4.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
  spot_light_count++;
  spot_lights[1] = SpotLight(1024, 1024, 
                            0.01f, 100.0f,
                            1.0f, 1.0f, 0.0f, 
                            0.0f, 1.0f, 
                            0.0f, -1.5f, 0.0f,
                            -100.0f, -1.0f, 0.0f,
                            1.0f, 0.0f, 0.0f,
                            20.0f);
  spot_light_count++;

  std::vector<std::string> skybox_faces;
  skybox_faces.push_back("src/Textures/Skybox/majestic_rt.tga");
  skybox_faces.push_back("src/Textures/Skybox/majestic_lf.tga");
  skybox_faces.push_back("src/Textures/Skybox/majestic_up.tga");
  skybox_faces.push_back("src/Textures/Skybox/majestic_dn.tga");
  skybox_faces.push_back("src/Textures/Skybox/majestic_bk.tga");
  skybox_faces.push_back("src/Textures/Skybox/majestic_ft.tga");

  skybox = Skybox(skybox_faces);

  glm::mat4 projection = glm::perspective(glm::radians(60.0f), main_window.GetBufferWidth() / main_window.GetBufferHeight(), 0.1f, 100.0f);

  // Loop until window closed
  while (!main_window.GetShouldClose()) {
    // Implement delta time functionality
    GLfloat now = glfwGetTime();
    delta_time = now - last_time;
    last_time = now;

    // Get + Handle user input events
    glfwPollEvents();

    camera.KeyControl(main_window.GetKeys(), delta_time);
    // TODO: Add this back in
    camera.MouseControl(main_window.GetXChange(), main_window.GetYChange());

    if (main_window.GetKeys()[GLFW_KEY_L]) {
      spot_lights[0].Toggle();
      main_window.GetKeys()[GLFW_KEY_L] = false;
    }
 
    DirectionalShadowMapPass(&main_light);
    for (size_t i = 0; i < point_light_count; i++) {
      OmniShadowMapPass(&point_lights[i]);
    }
    for (size_t i = 0; i < spot_light_count; i++) {
      OmniShadowMapPass(&spot_lights[i]);
    }
    RenderPass(projection, camera.CalcViewMatrix());

    glUseProgram(0);
    main_window.SwapBuffers();
  }

  return 0;
}