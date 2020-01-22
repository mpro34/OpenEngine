#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint HEIGHT = 600, WIDTH = 800;

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

  // Loop until window closed
  while (!glfwWindowShouldClose(main_window)) {
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(main_window);
  }


  return 0;
}