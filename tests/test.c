
#include "../include/glad.c"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../src/main.h"

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

int main()  {
	struct Window window;
	struct Scene scene;
	struct Camera camera;
	struct Mouse mouse;

	initWindow(&window, 1280, 720, "Rendering_library", 120);
	initScene(&scene, 4);
	initCamera(&camera, (vec3){0.0, 0.0, 10.0}, -90.0, 0.0, 90.0, 0.1, 1000.0);
	initMouse(&mouse, 0.2);

	generateSphere(&scene.models[0], 10, 10);
	generateSphere(&scene.models[1], 25, 25);
	generateSphere(&scene.models[2], 50, 50);
	generateSphere(&scene.models[3], 100, 100);
	initModel(&scene.models[0], 1);
	initModel(&scene.models[1], 1);
	initModel(&scene.models[2], 1);
	initModel(&scene.models[3], 1);

	scene.models[0].shader = createShaderProgram("./shaders/vertex.glsl", NULL, "./shaders/fragment.glsl");
	scene.models[1].shader = createShaderProgram("./shaders/vertex.glsl", NULL, "./shaders/fragment.glsl");
	scene.models[2].shader = createShaderProgram("./shaders/vertex.glsl", NULL, "./shaders/fragment.glsl");
	scene.models[3].shader = createShaderProgram("./shaders/vertex.glsl", NULL, "./shaders/fragment.glsl");

	initInstance(&scene.models[0].instances[0], (vec3){-7.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, 0.0);
	initInstance(&scene.models[1].instances[0], (vec3){-2.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, 0.0);
	initInstance(&scene.models[2].instances[0], (vec3){2.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, 0.0);
	initInstance(&scene.models[3].instances[0], (vec3){7.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, 0.0);

	window.userPtr.camera = &camera;
	window.userPtr.mouse = &mouse;

	scene.camera = &camera;

	float lastTime = glfwGetTime();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);
	while(!glfwWindowShouldClose(window.frame)) {	
		glClearColor(0.1, 0.0, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		delayFrame(window.lastFrame, window.targetFrameLength);
		updateDeltaTime(&window.currentFrame, &window.lastFrame, &window.deltaTime);

		//printf("Fps: %f\n", 1.0 / window.deltaTime);

		processKeyInput(&window, &camera);

		renderScene(&scene, &window);

		glfwSwapBuffers(window.frame);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

