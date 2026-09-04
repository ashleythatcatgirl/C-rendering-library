
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

	uint VAO;
	uint VBO;
	uint EBO;

	struct Vertex *verticies;
	uint *indices;

	uint vertexCount;
	uint indiceCount;
};

struct Scene {
	struct Model *models;
	int modelCount;

	struct Camera *camera;

	mat4 view;
	mat4 projection;
};

void initScene(struct Scene *scene, int modelCount);
void initModel(struct Model *model, int instanceCount);
void initInstance(struct ModelInstance *instance, vec3 position, vec3 rotation, vec3 scale, float rotationScale);

void generateModelArrays(struct Model *model);

void generateCube(struct Model *model, float scale);
struct Vertex *generateCubeVerticies(float scale);
uint *generateCubeIndices(void);

void generateSphere(struct Model *model, int verticalSlices, int horizontalSlices);

void floatToVec3(float a, vec3 dest);

#endif
