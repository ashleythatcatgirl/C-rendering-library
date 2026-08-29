
#include "GLFW/glfw3.h"

void processKeyInput(GLFWwindow *window, const float deltaTime) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}

	/*
	if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS
	&& window->delay < glfwGetTime() - 0.25) {
		window->delay = glfwGetTime();
		glfwSetInputMode(window, GLFW_CURSOR,
		glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED?
		GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
	*/

	float moveSpeed = 5.0 * deltaTime;
	float turnSpeed = 2.5 * deltaTime;
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) moveSpeed *= 5.0;

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) CameraMoveZ(camera, moveSpeed);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) CameraMoveZ(camera, -moveSpeed);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) CameraMoveX(camera, moveSpeed);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) CameraMoveX(camera, -moveSpeed);
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) CameraMoveY(camera, moveSpeed);
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) CameraMoveY(camera, -moveSpeed);

	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) CameraYaw(camera, turnSpeed, window);
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) CameraYaw(camera, -turnSpeed, window);
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) CameraPitch(camera, turnSpeed, window);
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) CameraPitch(camera, -turnSpeed, window);
	if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) CameraZoom(camera, 1);
	if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) CameraZoom(camera, -1);
}

