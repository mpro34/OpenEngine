#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window();
    Window(GLint window_width, GLint window_height);
    int Initialize();
    GLfloat GetBufferWidth() { return buffer_width; }
    GLfloat GetBufferHeight() { return buffer_height; }
    
    bool GetShouldClose() { return glfwWindowShouldClose(main_window); }
    void SwapBuffers() { glfwSwapBuffers(main_window); }

    ~Window();

  private:
    GLFWwindow *main_window;

    GLint width, height;
    GLint buffer_width, buffer_height;

};

// WINDOW_H_