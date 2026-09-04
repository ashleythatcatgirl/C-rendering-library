
#ifndef CONTROLS_H
#define CONTROLS_H

#include "GLFW/glfw3.h"

#include <stdbool.h>

struct Window;
struct WindowUserPointer;
struct Camera;
struct Mouse {
	double lastX;
	double lastY;

	float sensitivity;
	bool firstMouse;
};

void processKeyInput(struct Window *window, struct Camera *camera);
void toggleCursorMode(struct Window *window);
void initMouse(struct Mouse *mouse, const float sensitivity);

static inline bool isKeyPressed(GLFWwindow *frame, int key) {
	return glfwGetKey(frame, key) == GLFW_PRESS;
}




#endif
