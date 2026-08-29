
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "window.h"

GLFWwindow* createWindow(const int windowWidth, const int windowHeight, const char *windowName) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL) {
		printf("Failed to create window\n");
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to load GLAD\n");
		return NULL;
	}

	return window;
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
	glViewport(0, 0, width, height);
} 

/*
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	Controls *controls = glfwGetWindowUserPointer(window);
	if (controls->mouse->firstMouse) {
		controls->mouse->lastX = xPos;
		controls->mouse->lastY = yPos;

		controls->mouse->firstMouse = false;
	}

	float xOffset = xPos - controls->mouse->lastX;
	float yOffset = controls->mouse->lastY - yPos;
	xOffset *= 0.005;
	yOffset *= 0.005;

	controls->mouse->lastX = xPos;
	controls->mouse->lastY = yPos;
	
	CameraYaw(controls->camera, xOffset, window);
	CameraPitch(controls->camera, yOffset, window);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	Controls *controls = glfwGetWindowUserPointer(window);

	CameraZoom(controls->camera, 2*yOffset);
}
*/

