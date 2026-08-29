
#include "../include/glad.c"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../include/cglm/cglm.h"

#include "window.h"
#include "controls.h"
#include "shader.h"

float verticies[] = {
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5
};

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
	GLFWwindow *window = NULL;
	unsigned int shaderProgram;

 	window = createWindow(860, 540, "Rendering_library");

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	mat4 model, view, projection;

	vec3 cameraPos, cameraTarget, cameraUp, cameraFront;

	glm_vec3_copy((vec3){0.0, 3.0, 5.0}, cameraPos);
	glm_vec3_copy((vec3){0.0, -0.6, -1.0}, cameraFront);
	glm_normalize(cameraFront);

	/*
	glm_vec3_sub(cameraPos, cameraTarget, cameraDir);
	glm_normalize(cameraDir);

	vec3 up;
	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, up);
	glm_cross(up, cameraDir, cameraRight);
	glm_normalize(cameraRight);

	glm_cross(cameraDir, cameraRight, cameraUp);
	*/

	shaderProgram = createShaderProgram("../src/shaders/vertex.glsl", NULL, "../src/shaders/fragment.glsl");

	float deltaTime = 0, lastFrame = 0, currentFrame = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)) {	
		glClearColor(0.01, 0.0, 0.03, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyInput(window, deltaTime);

		glUseProgram(shaderProgram);

		glm_mat4_identity(model);
		glm_mat4_identity(view);
		glm_mat4_identity(projection);

		//glm_translate(model, (vec3){0.0, 0.0, 0.0});
		glm_rotate(model, (float)glfwGetTime(), (vec3){0.0, 1.0, 0.0});
		//glm_scale(model, (vec3){10.0, 10.0, 10.0});
		
		glm_vec3_add(cameraPos, cameraFront, cameraTarget);
		glm_lookat(cameraPos, cameraTarget, (vec3){0.0, 1.0, 0.0}, view);

		glm_perspective(glm_rad(90.0), 860.0/540.0, 0.1, 100.0, projection);

		shaderSetMat4(shaderProgram, "model", GL_FALSE, (float*)model);
		shaderSetMat4(shaderProgram, "view", GL_FALSE, (float*)view);
		shaderSetMat4(shaderProgram, "projection", GL_FALSE, (float*)projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

