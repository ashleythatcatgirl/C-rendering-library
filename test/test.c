
#include "../include/glad.c"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../src/main.h"

int main()  {
	struct Window window;
	struct Scene scene;
	struct Camera camera;
	struct Mouse mouse;

	initWindow(&window, 1280, 720, "Rendering_library", 120);
	initScene(&scene, 1);
	initCamera(&camera, (vec3){0.0, 0.0, 10.0}, -90.0, 0.0, 90.0, 0.1, 1000.0);
	initMouse(&mouse, 0.2);

	generateSphere(&scene.models[0], 20, 20);
	initModel(&scene.models[0], 1);

	scene.models[0].shader = createShaderProgram("./shaders/vertex.glsl", NULL, "./shaders/fragment.glsl");

	initInstance(&scene.models[0].instances[0], (vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, 0.0);

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

		while (glfwGetTime() < window.lastFrame + window.targetFrameLength) {}
		updateDeltaTime(&window);

		//printf("Fps: %f\n", 1.0 / window.deltaTime);

		processKeyInput(&window, &camera);

		renderScene(&scene, &window);

		glfwSwapBuffers(window.frame);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

