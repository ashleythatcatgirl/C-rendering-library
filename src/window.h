
#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"

struct Camera;
struct Mouse;
struct WindowUserPointer {
	struct Camera *camera;
	struct Mouse *mouse;
};

struct Window {
	GLFWwindow *frame;
	float width;
	float height;
	const char *name;

	struct WindowUserPointer userPtr;

	float currentFrame;
	float lastFrame;
	float deltaTime;

	float targetFps;
	float targetFrameLength;

	float tabDelay;
};

void initWindow(struct Window *window, const int width, const int height, const char *name, const float fps);
GLFWwindow *createWindow(const int height, const int width, const char *name, struct WindowUserPointer *userPointer);
void framebuffer_size_callback(GLFWwindow *frame, int width, int height);
void mouse_callback(GLFWwindow *frame, double xPos, double yPos);
void scroll_callback(GLFWwindow *frame, double xOffset, double yOffset);

void updateDeltaTime(struct Window *window);

#endif
