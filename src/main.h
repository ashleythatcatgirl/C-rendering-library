
#ifndef MAIN_H
#define MAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/cglm/cglm.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef WINDOW_H
#define WINDOW_H

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

	double currentFrame;
	double lastFrame;
	double deltaTime;

	float targetFps;
	float targetFrameLength;

	float tabDelay;
};

void initWindow(struct Window *window, const int width, const int height, const char *name, const float fps);
GLFWwindow *createWindow(const int height, const int width, const char *name, void *userPointer);
void framebuffer_size_callback(GLFWwindow *frame, int width, int height);
void mouse_callback(GLFWwindow *frame, double xPos, double yPos);
void scroll_callback(GLFWwindow *frame, double xOffset, double yOffset);

static void delayFrame(double lastFrame, float targetFrameLength) {
	while (glfwGetTime() < lastFrame + targetFrameLength) {}
}

static void updateDeltaTime(double *currentFrame, double *lastFrame, double *deltaTime) {
	*currentFrame = glfwGetTime();
	*deltaTime = *currentFrame - *lastFrame;
	*lastFrame = *currentFrame;
}

#endif // WINDOW_H

#ifndef SHADER_H 
#define SHADER_H

uint createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath);
void loadShader(const uint shaderProgram, const char *shaderPath, const int shaderType);
char *getShaderContent(const char *shaderFileName);
char *readFileToArray(FILE *fPtr);
void *resizeArray(void *array, uint size);
void freeShader(const char *shaderSource, uint shader);

static inline void shaderSetMat4(uint shaderProgram, const char *name, int gl_bool, mat4 data) {	
	int location = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(location, 1, gl_bool, (float*)data);
}
static inline void shaderSetVec3(uint shaderProgram, const char *name, vec3 data) {
	int location = glGetUniformLocation(shaderProgram, name);
	glUniform3fv(location, 1, (float*)data);
}
static inline void shaderSetVec2(uint shaderProgram, const char *name, vec2 data) {
	int location = glGetUniformLocation(shaderProgram, name);
	glUniform2fv(location, 1, (float*)data);
}
static inline void shaderSetFloat(uint shaderProgram, const char *name, float data) {
	int location = glGetUniformLocation(shaderProgram, name);
	glUniform1f(location, data);
}
static inline void shaderSetInt(uint shaderProgram, const char *name, int data) {
	int location = glGetUniformLocation(shaderProgram, name);
	glUniform1i(location, data);
}
static inline void shaderSetUInt(uint shaderProgram, const char *name, uint data) {
	int location = glGetUniformLocation(shaderProgram, name);
	glUniform1ui(location, data);
}

#endif // SHADER_H

#ifndef RENDER_H
#define RENDER_H

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

#endif // MAIN_H
