
#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"

GLFWwindow* createWindow(const int windowWidth, const int windowHeight, const char *windowName);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
