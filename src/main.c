
#include "../include/glad.c"
#include "camera.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>

#include "window.h"
#include "controls.h"
#include "shader.h"
#include "render.h"
#include "model.h"
#include "physics.h"

unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3,
	4, 5, 6,
	5, 6, 7,

	0, 1, 4,
	1, 4, 5,
	2, 3, 6,
	3, 6, 7,

	1, 3, 5,
        3, 5, 7,
	0, 2, 4,
	2, 4, 6
};

int main()  {
	struct Window window;

	window.width = 1280;
	window.height = 720;
 	window.frame = createWindow(window.width, window.height, "Rendering_library", &window.userPtr);

	struct Scene scene;
	scene.modelCount = 1;
	scene.models = malloc(sizeof(struct Model) * scene.modelCount);

	struct Model model;
	scene.models = &model;
	model.instanceCount = 4;
	model.instances = malloc(sizeof(struct ModelInstance) * model.instanceCount);

	model.vertexCount = 8;
	model.verticies = generateCube(1.0);

	model.indiceCount = sizeof(indices) / sizeof(unsigned int);
	model.indices = malloc(sizeof(unsigned int) * model.indiceCount);

	for (unsigned int i = 0; i < model.indiceCount; i++)
		model.indices[i] = indices[i];

	generateModelArrays(&model);

	model.shader = createShaderProgram("../src/shaders/vertex.glsl", NULL, "../src/shaders/fragment.glsl");

	model.instances[0].rotationScale = 0.0;
	model.instances[1].rotationScale = 0.0;
	model.instances[2].rotationScale = 0.0;

	glm_vec3_copy((vec3){0.0, 0.0, 0.0}, model.instances[0].position);
	glm_vec3_copy((vec3){0.0, 1.0, 0.2}, model.instances[0].rotation);
	glm_vec3_copy((vec3){10.0, 10.0, 10.0}, model.instances[0].scale);

	glm_vec3_copy((vec3){50.0, 0.0, 0.0}, model.instances[1].position);
	glm_vec3_copy((vec3){0.0, 1.0, 0.2}, model.instances[1].rotation);
	glm_vec3_copy((vec3){1.0, 1.0, 1.0}, model.instances[1].scale);

	glm_vec3_copy((vec3){25.0, 0.0, 0.0}, model.instances[2].position);
	glm_vec3_copy((vec3){0.0, 1.0, 0.2}, model.instances[2].rotation);
	glm_vec3_copy((vec3){0.5, 0.5, 0.5}, model.instances[2].scale);

	glm_vec3_copy((vec3){50.0, 0.0, 5.0}, model.instances[3].position);
	glm_vec3_copy((vec3){0.0, 1.0, 0.2}, model.instances[3].rotation);
	glm_vec3_copy((vec3){0.2, 0.2, 0.2}, model.instances[3].scale);

	struct Camera camera;
	initCamera(&camera, (vec3){0.0, 100.0, 0.0}, -90.0, -89.0, 90.0);
	struct Mouse mouse;
	mouse.sensitivity = 0.2;
	mouse.firstMouse = true;
	window.userPtr.camera = &camera;
	window.userPtr.mouse = &mouse;

	scene.camera = &camera;

	struct Body *bodies;
	int bodyCount = 4;
	bodies = malloc(sizeof(struct Body) * bodyCount);

	initBody(&bodies[0], (vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, 10000.0, 10.0);
	initBody(&bodies[1], (vec3){50.0, 0.0, 0.0}, (vec3){0.0, 0.0, 15.0}, 100.0, 1.0);
	initBody(&bodies[2], (vec3){25.0, 0.0, 0.0}, (vec3){0.0, 0.0, 20.0}, 10.0, 0.5);
	initBody(&bodies[3], (vec3){50.0, 0.0, 5.0}, (vec3){4.0, 0.0, 15.0}, 5.0, 0.5);

	float lastTime = glfwGetTime();
	float targetFrameLength = 1.0 / 120.0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);
	while(!glfwWindowShouldClose(window.frame)) {	
		glClearColor(0.01, 0.0, 0.03, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updateDeltaTime(&window);

		//printf("Fps: %f\n", 1.0 / window.deltaTime);

		processKeyInput(&window, &camera);

		updatePlanets(bodies, bodyCount, window.deltaTime);

		for (unsigned int i = 0; i < model.instanceCount; i++) {
			glm_vec3_copy(bodies[i].position, model.instances[i].position);
			model.instances[i].rotationScale = glfwGetTime() * (1.0 / bodies[i].radius);
		}

		renderScene(&scene, &window);
	
		while (glfwGetTime() < lastTime + targetFrameLength) {}
		lastTime += targetFrameLength;

		glfwSwapBuffers(window.frame);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

