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

    bool* GetKeys() { return keys; }
    GLfloat GetXChange();
    GLfloat GetYChange();

    ~Window();

  private:
    GLFWwindow *main_window;

    GLint width, height;
    GLint buffer_width, buffer_height;

    bool keys[1024];

    GLfloat last_x = 0.0f;
    GLfloat last_y = 0.0f;
    GLfloat x_change = 0.0f;
    GLfloat y_change = 0.0f;
    bool mouse_first_moved;

    void CreateCallbacks();
    static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void HandleMouse(GLFWwindow *window, double x_pos, double y_pos);
};

// WINDOW_H_