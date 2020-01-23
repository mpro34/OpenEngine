#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

// Window dimensions
const GLint HEIGHT = 600, WIDTH = 800;
const float to_radians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniform_model, uniform_projection;

bool direction = true;
float tri_offset = 0.0f;
float tri_max_offset = 0.7f;
float tri_increment = 0.0005f;

float cur_angle = 0.0f;

bool size_direction = true;
float cur_size = 0.4f;
float max_size = 0.8f;
float min_size = 0.1f;

// Vertex shader
static const char* v_shader = "                                     \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
out vec4 vert_color;                                                \n\
                                                                    \n\
uniform mat4 model;                                                 \n\
uniform mat4 projection;                                            \n\
                                                                    \n\
void main() {                                                       \n\
  gl_Position = projection * model * vec4(pos, 1.0f);               \n\
  vert_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                  \n\
}";

// Fragment Shader
static const char* f_shader = "                            \n\
#version 330                                               \n\
                                                           \n\
in vec4 vert_color;                                        \n\
                                                           \n\
out vec4 color;                                            \n\
                                                           \n\
void main() {                                              \n\
  color = vert_color;                                      \n\
}";

void CreateTriangle() {

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

  glGenVertexArrays(1, &VAO); 
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  
  // Unbind buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbind the VBO
  glBindVertexArray(0);                     // unbind the VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO (must happen after unbinding the VAO!)
}

void AddShader(GLuint program, const char* shader_code, GLenum shader_type) {
  GLuint the_shader = glCreateShader(shader_type);

  const GLchar* the_code[1];
  the_code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = strlen(shader_code);

  glShaderSource(the_shader, 1, the_code, code_length);
  glCompileShader(the_shader);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
  if(!result) {
    glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: '%s'\n", shader_type, eLog);
    return;
  }

  glAttachShader(program, the_shader);
}

void CompileShaders() {
  shader = glCreateProgram();
  if (!shader) {
    printf("Error creating shader program!\n");
    return;
  }

  AddShader(shader, v_shader, GL_VERTEX_SHADER);
  AddShader(shader, f_shader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if(!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if(!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }

  uniform_model = glGetUniformLocation(shader, "model");
  uniform_projection = glGetUniformLocation(shader, "projection");
}


int main() {
  // Initialize glfw
  if (!glfwInit()) {
    printf("GLFW Initialization failed!");
    glfwTerminate();
    return 1;
  }

  // Setup GLFW window properties
  // OpenGL Version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile = No backwards compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *main_window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
  if (!main_window) {
    printf("GFLW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get Buffer size information
  int buffer_width, buffer_height;
  glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

  // Set context for GLEW to use
  glfwMakeContextCurrent(main_window);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if(glewInit() != GLEW_OK) {
    printf("GLEW initialization failed!");
    glfwDestroyWindow(main_window);
    glfwTerminate();
    return 1;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup Viewport size
  glViewport(0, 0, buffer_width, buffer_height);

  CreateTriangle();
  CompileShaders();

  glm::mat4 projection = glm::perspective(45.0f, (GLfloat)buffer_width / (GLfloat)buffer_height, 0.1f, 100.0f);

  // Loop until window closed
  while (!glfwWindowShouldClose(main_window)) {
    // Get + Handle user input events
    glfwPollEvents();

    if (direction) {
      tri_offset += tri_increment;
    } else {
      tri_offset -= tri_increment;
    }

    cur_angle += 0.01f;
    if (cur_angle >= 360.0f) {
      cur_angle -= 360.0f;
    }

    if (abs(tri_offset) >= tri_max_offset) {
      direction = !direction;
    }

    if (size_direction) {
      cur_size += 0.0001f;
    } else {
      cur_size -= 0.0001f;
    }

    if (cur_size >= max_size || cur_size <= min_size) {
      size_direction = !size_direction;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indents indicate levels of drawing and clearing buffers at the end
    glUseProgram(shader);

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(tri_offset, 0.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      // model = glm::rotate(model, cur_angle * to_radians, glm::vec3(0.0f, 1.0f, 0.0f));

      // glUniform1f(uniform_model, tri_offset);
      glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));

      glBindVertexArray(VAO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);     
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    glfwSwapBuffers(main_window);
  }


  return 0;
}