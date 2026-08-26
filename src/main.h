
#ifndef MAIN_H
#define MAIN_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

GLFWwindow* CreateWindow(const int windowWidth, const int windowHeight, const char *windowName);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
