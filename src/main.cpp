#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint HEIGHT = 600, WIDTH = 800;

GLuint VAO, VBO, shader;

// Vertex shader
static const char* v_shader = "                            \n\
#version 330                                               \n\
                                                           \n\
layout (location = 0) in vec3 pos;                         \n\
                                                           \n\
void main() {                                              \n\
  gl_Position = vec4(0.4*pos.x, 0.4*pos.y, pos.z, 1.0);    \n\
}";

// Fragment Shader
static const char* f_shader = "                            \n\
#version 330                                               \n\
                                                           \n\
out vec4 color;                                            \n\
                                                           \n\
void main() {                                              \n\
  color = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}";

void CreateTriangle() {
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };

  glGenVertexArrays(1, &VAO); 
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  // Unbind buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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

  // Setup Viewport size
  glViewport(0, 0, buffer_width, buffer_height);

  CreateTriangle();
  CompileShaders();

  // Loop until window closed
  while (!glfwWindowShouldClose(main_window)) {
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

  // Indents indicate levels of drawing and clearing buffers at the end
    glUseProgram(shader);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);     
    glUseProgram(0);

    glfwSwapBuffers(main_window);
  }


  return 0;
}