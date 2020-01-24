#include "../headers/Window.h"

Window::Window() {
  width = 800;
  height = 600;

  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
}

Window::Window(GLint window_width, GLint window_height) {
  width = window_width;
  height = window_height;

  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
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

  // Handle key and mouse input
  CreateCallbacks();
  glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  glfwSetWindowUserPointer(main_window, this);
}

void Window::CreateCallbacks() {
  glfwSetKeyCallback(main_window, HandleKeys);
  glfwSetCursorPosCallback(main_window, HandleMouse);
}

GLfloat Window::GetXChange() {
  GLfloat the_change = x_change;
  x_change = 0.0f;
  return the_change;
}

GLfloat Window::GetYChange() {
  GLfloat the_change = y_change;
  y_change = 0.0f;
  return the_change;
}

void Window::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
  Window *the_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      the_window->keys[key] = true;
    } 
    else if (action == GLFW_RELEASE) {
      the_window->keys[key] = false;
    }
  }
}

void Window::HandleMouse(GLFWwindow *window, double x_pos, double y_pos) {
  // Handle mouse movement
  Window *the_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (the_window->mouse_first_moved) {
    the_window->last_x = x_pos;
    the_window->last_y = y_pos;
    the_window->mouse_first_moved = false;
  }

  the_window->x_change = x_pos - the_window->last_x;
  the_window->y_change = the_window->last_y - y_pos;

  the_window->last_x = x_pos;
  the_window->last_y = y_pos;
}

Window::~Window() {
  glfwDestroyWindow(main_window);
  glfwTerminate();
}