
#include "GLFW/glfw3.h"

#include <stdbool.h>

#include "camera.h"
#include "window.h"
#include "controls.h"

void processKeyInput(struct Window *window, struct Camera *camera) {
	GLFWwindow *frame = window->frame;

	if(isKeyPressed(frame, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(frame, 1);

	if(isKeyPressed(frame, GLFW_KEY_TAB) && window->tabDelay < glfwGetTime())
		toggleCursorMode(window);

	float moveSpeed = 5.0 * window->deltaTime;
	float turnSpeed = 75.0 * window->deltaTime;
	float zoomSpeed = 50.0 * window->deltaTime;

	if(isKeyPressed(frame, GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 5.0;

	if(isKeyPressed(frame, GLFW_KEY_W))
		cameraMoveZ(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_S))
		cameraMoveZ(camera, -moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_A))
		cameraMoveX(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_D))
		cameraMoveX(camera, -moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_E))
		cameraMoveY(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_Q))
		cameraMoveY(camera, -moveSpeed);

	if(isKeyPressed(frame, GLFW_KEY_RIGHT))
		cameraTurnYaw(camera,  turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_LEFT))
		cameraTurnYaw(camera, -turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_UP))
		cameraTurnPitch(camera,  turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_DOWN))
		cameraTurnPitch(camera, -turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_C))
		cameraZoom(camera,  zoomSpeed);
	if(isKeyPressed(frame, GLFW_KEY_Z))
		cameraZoom(camera, -zoomSpeed);
}

void toggleCursorMode(struct Window *window) {
	window->tabDelay = glfwGetTime() + 0.1;
	glfwSetInputMode(window->frame, GLFW_CURSOR,
		glfwGetInputMode(window->frame, GLFW_CURSOR) == GLFW_CURSOR_DISABLED?
		GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
	);

	if (glfwGetInputMode(window->frame, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return;

	struct WindowUserPointer *windowPtr = glfwGetWindowUserPointer(window->frame);
	windowPtr->mouse->firstMouse = true;
}

void initMouse(struct Mouse *mouse, const float sensitivity) {
	mouse->firstMouse = true;
	mouse->sensitivity = sensitivity;
}

