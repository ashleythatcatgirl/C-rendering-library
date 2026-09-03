
#ifndef MODEL_H
#define MODEL_H

#include "../include/cglm/cglm.h"

struct Vertex {
	vec3 position;
};

struct ModelInstance {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	float rotationScale;

	mat4 model;
};

struct Model {
	struct ModelInstance *instances;
	int instanceCount;
	
	int shader;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	struct Vertex *verticies;
	unsigned int *indices;

	unsigned int vertexCount;
	unsigned int indiceCount;
};

struct Scene {
	struct Model *models;
	int modelCount;

	struct Camera *camera;

	mat4 view;
	mat4 projection;
};

void generateModelArrays(struct Model *model);
struct Vertex *generateCube(float scale);

#endif
