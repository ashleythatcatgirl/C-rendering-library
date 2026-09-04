
#include "glad/glad.h"

#include "render.h"
#include "model.h"
#include "shader.h"
#include "window.h"
#include "camera.h"

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

	shaderSetMat4(model->shader, "view", GL_FALSE, (float*)viewMat);
	shaderSetMat4(model->shader, "projection", GL_FALSE, (float*)projectionMat);

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
		
	shaderSetMat4(shaderProgram, "model", GL_FALSE, (float*)instance->model);

	glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
}
