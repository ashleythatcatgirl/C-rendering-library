
#include "camera.h"
#include "controls.h"

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
