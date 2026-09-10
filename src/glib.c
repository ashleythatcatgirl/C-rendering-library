
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include "glib.h"

// WINDOW_C

void initWindow(struct Window *window, const int width, const int height, const char *name, const float fps) {
	window->width = width;
	window->height = height;
	window->name = name;
	window->targetFps = fps;
	window->targetFrameLength = 1.0 / fps;

	window->frame = createWindow(window->width, window->height, window->name, (void*)&window->userPtr);
}

GLFWwindow *createWindow(const int width, const int height, const char *name, void *userPointer) {
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

	glfwSetWindowUserPointer(frame, userPointer);

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

// SHADER_C

uint createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath) {
	uint shaderProgram = glCreateProgram();

	if (vertexShaderPath) loadShader(shaderProgram, vertexShaderPath, GL_VERTEX_SHADER);
	if (geometryShaderPath) loadShader(shaderProgram, geometryShaderPath, GL_GEOMETRY_SHADER);
	if (fragmentShaderPath) loadShader(shaderProgram, fragmentShaderPath, GL_FRAGMENT_SHADER);

	return shaderProgram;
}

void loadShader(const uint shaderProgram, const char *shaderPath, const int shaderType) {
	uint shader;
	int success;

	const char *shaderContent = getShaderContent(shaderPath);
	if (!shaderContent) return;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderContent, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		printf("Shader compilation error\nError log\n%s\nEnd\n", infoLog);

		goto freeShaderMemory;
	}

	glAttachShader(shaderProgram, shader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    	if (!success) {
		char infoLog[1024];
        	glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        	printf("Shader linking error\nError log\n%s\nEnd\n", infoLog);

		goto freeShaderMemory;
    	}

freeShaderMemory:

	freeShader(shaderContent, shader);
}

char *getShaderContent(const char *shaderFileName) {
	FILE *fPtr = fopen(shaderFileName, "r");
	if (!fPtr)
		return NULL;

	char *shaderContent = readFileToArray(fPtr);

	fclose(fPtr);

	return shaderContent;
}

char *readFileToArray(FILE *fPtr) {
	int size = 1024;
	char *array = malloc(sizeof(char) * size);
	if (!array)
		return NULL;

	char buffer = 0;
	int i = 0;
	for (; (buffer = fgetc(fPtr)) != EOF; i++) {
		if (i >= size) {
			size *= 2;
			array = (char*)resizeArray(array, sizeof(char) * size);
		}
		
		array[i] = buffer;
	}

	array[i] = '\0';

	return array;
}

void* resizeArray(void *array, uint size) {
	void *temp = realloc(array, size);
	return temp;
}

void freeShader(const char *shaderSource, uint shader) {
	free((char*)shaderSource);
	glDeleteShader(shader);
}

// RENDER_C

void renderScene(struct Scene *scene, struct Window *window) {
	glm_mat4_identity(scene->view);
	glm_mat4_identity(scene->projection);

	glm_vec3_add(scene->camera->position, scene->camera->front, scene->camera->target);
	glm_lookat(scene->camera->position, scene->camera->target, scene->camera->up, scene->view);

	glm_perspective(glm_rad(scene->camera->fov), window->width/window->height, scene->camera->nearPlane, scene->camera->farPlane, scene->projection);

	for (uint i = 0; i < scene->modelCount; i++)
		renderModel(&scene->models[i], scene->view, scene->projection);
}

void renderModel(struct Model *model, mat4 viewMat, mat4 projectionMat) {
	glUseProgram(model->shader);

	shaderSetMat4(model->shader, "view", GL_FALSE, viewMat);
	shaderSetMat4(model->shader, "projection", GL_FALSE, projectionMat);

	glBindVertexArray(model->VAO);

	for (uint i = 0; i < model->instanceCount; i++)
		renderModelInstance(&model->instances[i], model->shader, model->indiceCount);

	glBindVertexArray(0);
}

void renderModelInstance(struct ModelInstance *instance, uint shaderProgram, uint indiceCount) {
	glm_mat4_identity(instance->model);

	glm_translate(instance->model, instance->position);
	glm_rotate(instance->model, instance->rotationScale, instance->rotation);
	glm_scale(instance->model, instance->scale);
		
	shaderSetMat4(shaderProgram, "model", GL_FALSE, instance->model);

	glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
}

// MODEL_C

void initScene(struct Scene *scene, int modelCount) {
	scene->modelCount = modelCount;
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

// CONTROLS_C

void processKeyInput(struct Window *window, struct Camera *camera) {
	GLFWwindow *frame = window->frame;

	if(isKeyPressed(frame, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(frame, 1);

	if(isKeyPressed(frame, GLFW_KEY_TAB) && window->tabDelay < glfwGetTime())
		toggleCursorMode(window);

	float moveSpeed = 5.0 * window->deltaTime;
	float turnSpeed = 75.0 * window->deltaTime;
	float zoomSpeed = 50.0 * window->deltaTime;

	if(isKeyPressed(frame, GLFW_KEY_LEFT_SHIFT))
		moveSpeed *= 5.0;

	if(isKeyPressed(frame, GLFW_KEY_W))
		cameraMoveZ(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_S))
		cameraMoveZ(camera, -moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_A))
		cameraMoveX(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_D))
		cameraMoveX(camera, -moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_E))
		cameraMoveY(camera,  moveSpeed);
	if(isKeyPressed(frame, GLFW_KEY_Q))
		cameraMoveY(camera, -moveSpeed);

	if(isKeyPressed(frame, GLFW_KEY_RIGHT))
		cameraTurnYaw(camera,  turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_LEFT))
		cameraTurnYaw(camera, -turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_UP))
		cameraTurnPitch(camera,  turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_DOWN))
		cameraTurnPitch(camera, -turnSpeed);
	if(isKeyPressed(frame, GLFW_KEY_C))
		cameraZoom(camera,  zoomSpeed);
	if(isKeyPressed(frame, GLFW_KEY_Z))
		cameraZoom(camera, -zoomSpeed);
}

void toggleCursorMode(struct Window *window) {
	window->tabDelay = glfwGetTime() + 0.1;
	glfwSetInputMode(window->frame, GLFW_CURSOR,
		glfwGetInputMode(window->frame, GLFW_CURSOR) == GLFW_CURSOR_DISABLED?
		GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
	);

	if (glfwGetInputMode(window->frame, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return;

	struct WindowUserPointer *windowPtr = glfwGetWindowUserPointer(window->frame);
	windowPtr->mouse->firstMouse = true;
}

void initMouse(struct Mouse *mouse, const float sensitivity) {
	mouse->firstMouse = true;
	mouse->sensitivity = sensitivity;
}

// CAMERA_C

void cameraMoveX(struct Camera *camera, float moveSpeed) {
	vec3 move;
	glm_vec3_cross(camera->up, camera->front, camera->right);
	glm_normalize(camera->right);
	glm_vec3_scale(camera->right, moveSpeed, move);
	glm_vec3_add(camera->position, move, camera->position);
}

void cameraMoveY(struct Camera *camera, float moveSpeed) {
	vec3 move;
	glm_vec3_scale(camera->up, moveSpeed, move);
	glm_vec3_add(camera->position, move, camera->position);
}

void cameraMoveZ(struct Camera *camera, float moveSpeed) {
	vec3 move;
	glm_vec3_scale(camera->front, moveSpeed, move);
	glm_vec3_add(camera->position, move, camera->position);
}

void cameraTurnYaw(struct Camera *camera, float turnSpeed) {
	camera->yaw += turnSpeed;

	glm_vec3_copy((vec3){
		cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
		sin(glm_rad(camera->pitch)),
		sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
	}, camera->front);

	glm_normalize(camera->front);
}

void cameraTurnPitch(struct Camera *camera, float turnSpeed) {
	if (camera->pitch + turnSpeed >= 90.0 || camera->pitch + turnSpeed <= -90.0)
		return;

	camera->pitch += turnSpeed;

	glm_vec3_copy((vec3){
		cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
		sin(glm_rad(camera->pitch)),
		sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
	}, camera->front);

	glm_normalize(camera->front);
}

void cameraTurnRoll(struct Camera *camera, float turnSpeed) {

}

void cameraZoom(struct Camera *camera, float zoomSpeed) {
	if (camera->fov - zoomSpeed >= 180 || camera->fov - zoomSpeed <= 0)
		return;

	camera->fov -= zoomSpeed;
}

void initCamera(struct Camera *camera, vec3 position, float yaw, float pitch, float fov, float nearPlane, float farPlane) {
	glm_vec3_copy(position, camera->position);
	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, camera->up);

	camera->yaw = yaw;
	camera->pitch = pitch;
	camera->roll = 0.0;

	camera->fov = fov;

	camera->nearPlane = 0.1;
	camera->farPlane = 1000.0;

	glm_vec3_copy((vec3){
		cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
		sin(glm_rad(camera->pitch)),
		sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
	}, camera->front);

	glm_normalize(camera->front);
}
