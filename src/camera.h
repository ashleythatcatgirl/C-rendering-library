
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
};

void initCamera(struct Camera *camera, vec3 position, float yaw, float pitch, float fov);
void cameraMoveX(struct Camera *camera, float moveSpeed);
void cameraMoveY(struct Camera *camera, float moveSpeed);
void cameraMoveZ(struct Camera *camera, float moveSpeed);
void cameraTurnYaw(struct Camera *camera, float turnSpeed);
void cameraTurnPitch(struct Camera *camera, float turnSpeed);
void cameraTurnRoll(struct Camera *camera, float turnSpeed);
void cameraZoom(struct Camera *camera, float zoomSpeed);

#endif
