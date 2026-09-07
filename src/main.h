
#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"

struct Camera;
struct Mouse;
struct WindowUserPointer {
	struct Camera *camera;
	struct Mouse *mouse;
};

struct Window {
	GLFWwindow *frame;
	float width;
	float height;
	const char *name;

	struct WindowUserPointer userPtr;

	float currentFrame;
	float lastFrame;
	float deltaTime;

	float targetFps;
	float targetFrameLength;

	float tabDelay;
};

void initWindow(struct Window *window, const int width, const int height, const char *name, const float fps);
GLFWwindow *createWindow(const int height, const int width, const char *name, struct WindowUserPointer *userPointer);
void framebuffer_size_callback(GLFWwindow *frame, int width, int height);
void mouse_callback(GLFWwindow *frame, double xPos, double yPos);
void scroll_callback(GLFWwindow *frame, double xOffset, double yOffset);

void updateDeltaTime(struct Window *window);

#endif // WINDOW_H

#ifndef SHADER_H 
#define SHADER_H

#include "stdlib.h"
#include "stdio.h"

uint createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath);
void loadShader(const uint shaderProgram, const char *shaderPath, const int shaderType);
char *getShaderContent(const char *shaderFileName);
char *readFileToArray(FILE *fPtr);
void *resizeArray(void *array, uint size);
void freeShader(const char *shaderSource, uint shader);

void shaderSetMat4(uint shaderProgram, const char *name, int gl_bool, float *data);

#endif // SHADER_H

#ifndef RENDER_H
#define RENDER_H

#include "../include/cglm/cglm.h"

struct Scene;
struct Model;
struct Window;
struct ModelInstance;

void renderScene(struct Scene *scene, struct Window *window);
void renderModel(struct Model *model, mat4 viewMat, mat4 projectionMat);
void renderModelInstance(struct ModelInstance *instance, uint shaderProgram, uint indiceCount);

#endif // RENDER_H

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

#endif // MODEL_H

#ifndef CONTROLS_H
#define CONTROLS_H

#include "GLFW/glfw3.h"

#include <stdbool.h>

struct Window;
struct WindowUserPointer;
struct Camera;
struct Mouse {
	double lastX;
	double lastY;

	float sensitivity;
	bool firstMouse;
};

void processKeyInput(struct Window *window, struct Camera *camera);
void toggleCursorMode(struct Window *window);
void initMouse(struct Mouse *mouse, const float sensitivity);

static inline bool isKeyPressed(GLFWwindow *frame, int key) {
	return glfwGetKey(frame, key) == GLFW_PRESS;
}

#endif // CONTROLS_H

#ifndef CAMERA_H
#define CAMERA_H

#include "GLFW/glfw3.h"
#include "../include/cglm/cglm.h"

struct Camera {
	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;
	vec3 target;

	float yaw;
	float pitch;
	float roll;

	float fov;

	float nearPlane;
	float farPlane;
};

void initCamera(struct Camera *camera, vec3 position, float yaw, float pitch, float fov, float nearPlane, float farPlane);
void cameraMoveX(struct Camera *camera, float moveSpeed);
void cameraMoveY(struct Camera *camera, float moveSpeed);
void cameraMoveZ(struct Camera *camera, float moveSpeed);
void cameraTurnYaw(struct Camera *camera, float turnSpeed);
void cameraTurnPitch(struct Camera *camera, float turnSpeed);
void cameraTurnRoll(struct Camera *camera, float turnSpeed);
void cameraZoom(struct Camera *camera, float zoomSpeed);

#endif // CAMERA_H
