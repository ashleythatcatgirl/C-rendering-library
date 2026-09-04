
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>

#include "window.h"
#include "camera.h"
#include "controls.h"

void initWindow(struct Window *window, const int width, const int height, const char *name, const float fps) {
	window->width = width;
	window->height = height;
	window->name = name;
	window->targetFps = fps;
	window->targetFrameLength = 1.0 / fps;

	window->frame = createWindow(window->width, window->height, window->name, &window->userPtr);
}

GLFWwindow *createWindow(const int width, const int height, const char *name, struct WindowUserPointer *userPointer) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *frame = glfwCreateWindow(width, height, name, NULL, NULL);
	if (frame == NULL) {
		printf("Failed to create window\n");
		return NULL;
	}

	glfwSetWindowUserPointer(frame, (void*)userPointer);

	glfwMakeContextCurrent(frame);
	glfwSetFramebufferSizeCallback(frame, framebuffer_size_callback);  
	glfwSetCursorPosCallback(frame, mouse_callback);
	glfwSetScrollCallback(frame, scroll_callback);

	glfwSetInputMode(frame, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to load GLAD\n");
		return NULL;
	}

	return frame;
}

void framebuffer_size_callback(GLFWwindow *frame, const int width, const int height) {
	glViewport(0, 0, width, height);
} 

void mouse_callback(GLFWwindow* frame, double xPos, double yPos) {
	if (glfwGetInputMode(frame, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return;

	struct WindowUserPointer *windowPtr = glfwGetWindowUserPointer(frame);
	struct Mouse *mouse = windowPtr->mouse;
	struct Camera *camera = windowPtr->camera;

	if (mouse->firstMouse) {
		mouse->lastX = xPos;
		mouse->lastY = yPos;

		mouse->firstMouse = false;
	}

	float xOffset = xPos - mouse->lastX;
	float yOffset = mouse->lastY - yPos;
	xOffset *= mouse->sensitivity;
	yOffset *= mouse->sensitivity;

	mouse->lastX = xPos;
	mouse->lastY = yPos;

	cameraTurnYaw(camera, xOffset);
	cameraTurnPitch(camera, yOffset);
}

void scroll_callback(GLFWwindow *frame, double xOffset, double yOffset) {
	struct WindowUserPointer *windowPtr = glfwGetWindowUserPointer(frame);
	struct Camera *camera = windowPtr->camera;

	cameraZoom(camera, 2 * yOffset);
}

void updateDeltaTime(struct Window *window) {
	window->currentFrame = glfwGetTime();
	window->deltaTime = window->currentFrame - window->lastFrame;
	window->lastFrame = window->currentFrame;

}
