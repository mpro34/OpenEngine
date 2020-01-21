#include <stdio.h>

#include <gl\GLU.h>
#include <GLFW\glfw3.h>
#include <iostream>

// Window dimensions
const GLuint height = 600, width = 800;

int main() {
  // Initialize glfw
  if (!glfwInit()) {
    printf("GLFW Initialization failed!");
    glfwTerminate();
    return 1;
  }

  return 0;
}