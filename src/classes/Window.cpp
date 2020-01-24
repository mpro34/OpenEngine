#include "../headers/Window.h"

Window::Window() {
  width = 800;
  height = 600;
}

Window::Window(GLint window_width, GLint window_height) {
  width = window_width;
  height = window_height;
}

int Window::Initialize() {
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

  main_window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
  if (!main_window) {
    printf("GFLW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get Buffer size information
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
}

Window::~Window() {
  glfwDestroyWindow(main_window);
  glfwTerminate();
}