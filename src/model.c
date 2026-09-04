
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include "model.h"

void initScene(struct Scene *scene, int modelCount) {
	scene->modelCount = 1;
	scene->models = malloc(sizeof(struct Model) * scene->modelCount);
}

void initModel(struct Model *model, int instanceCount) {
	generateModelArrays(model);

	model->instanceCount = instanceCount;
	model->instances = malloc(sizeof(struct ModelInstance) * model->instanceCount);
}

void initInstance(struct ModelInstance *instance, vec3 position, vec3 rotation, vec3 scale, float rotationScale) {
	if (position)
		glm_vec3_copy(position, instance->position);
	else
		glm_vec3_zero(instance->position);

	if (rotation)
		glm_vec3_copy(rotation, instance->rotation);
	else
		glm_vec3_zero(instance->rotation);

	if (scale)
		glm_vec3_copy(scale, instance->scale);
	else
		glm_vec3_zero(instance->scale);

	instance->rotationScale = rotationScale;
}

void generateModelArrays(struct Model *model) {
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);

	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * model->vertexCount, &model->verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * model->indiceCount, model->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void generateCube(struct Model *model, float scale) {
	model->vertexCount = 8;
	model->indiceCount = 36;

	model->verticies = generateCubeVerticies(scale);
	model->indices = generateCubeIndices();
}

struct Vertex *generateCubeVerticies(float scale) {
	struct Vertex *verticies;
	verticies = malloc((sizeof(struct Vertex) * 8));

	float edge = scale / 2;

	glm_vec3_copy((vec3){-edge, -edge, edge}, verticies[0].position);
	glm_vec3_copy((vec3){edge, -edge, edge}, verticies[1].position);
	glm_vec3_copy((vec3){-edge, edge, edge}, verticies[2].position);
	glm_vec3_copy((vec3){edge, edge, edge}, verticies[3].position);
	glm_vec3_copy((vec3){-edge, -edge, -edge}, verticies[4].position);
	glm_vec3_copy((vec3){edge, -edge, -edge}, verticies[5].position);
	glm_vec3_copy((vec3){-edge, edge, -edge}, verticies[6].position);
	glm_vec3_copy((vec3){edge, edge, -edge}, verticies[7].position);

	return verticies;
}

uint *generateCubeIndices(void) {
	uint *indices;
	indices = malloc(sizeof(uint) * 36);
	
	uint temp[] = {
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

	for (uint i = 0; i < 36; i++)
		indices[i] = temp[i];

	return indices;
}

void generateSphere(struct Model *model, int verticalSlices, int horizontalSlices) {
	model->vertexCount = verticalSlices * (horizontalSlices - 1) + 2;
	model->verticies = malloc(sizeof(struct Vertex) * model->vertexCount);

	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, model->verticies[0].position);

	uint m = 1;
	for (uint h = 0; h < horizontalSlices - 1; h++) {
		float phi = (h + 1) * (M_PI / horizontalSlices);

		for (uint v = 0; v < verticalSlices; v++) {
			float theta = v * (2.0 * M_PI / verticalSlices);

			glm_vec3_copy((vec3){sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta)}, model->verticies[m].position);
			m++;
		}
	}

	glm_vec3_copy((vec3){0.0, -1.0, 0.0}, model->verticies[m].position);

	model->indiceCount = verticalSlices * (horizontalSlices - 1) * 6 + 3;
	model->indices = malloc(sizeof(uint) * model->indiceCount);

	uint x = 1;
	uint y = x + horizontalSlices;
	m = 0;

	// Add the last, first layer triangle
	// TODO; FIX THIS AWFUL CODE
	model->indices[m++] = 1;
	model->indices[m++] = verticalSlices;
	model->indices[m++] = y;

	for (uint i = 1; i < (horizontalSlices - 1); i++) {
		for (uint j = 0; j < verticalSlices; j++) {
			y = x + horizontalSlices;
			model->indices[m++] = x;
			model->indices[m++] = x + 1;
			model->indices[m++] = y;

			model->indices[m++] = x + 1;
			model->indices[m++] = y;
			model->indices[m++] = y + 1;
			x++;
		}
	}

	// Add top triangles
	x = 1;
	for (uint i = 0; i < verticalSlices - 1; i++) {
		model->indices[m++] = 0;
		model->indices[m++] = x;
		model->indices[m++] = x + 1;
		x++;
	}

	model->indices[m++] = 0;
	model->indices[m++] = 1;
	model->indices[m++] = x;

	// Add bottom triangles
	x = model->vertexCount - 2;
	for (uint i = 0; i < verticalSlices - 1; i++) {
		model->indices[m++] = model->vertexCount - 1;
		model->indices[m++] = x;
		model->indices[m++] = x - 1;
		x--;
	}

	model->indices[m++] = model->vertexCount - 1;
	model->indices[m++] = model->vertexCount - 2;
	model->indices[m++] = x;
}

void floatToVec3(float a, vec3 dest) {
	glm_vec3_copy((vec3){a, a, a}, dest);
}
