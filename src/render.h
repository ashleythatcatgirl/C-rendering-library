
#ifndef RENDER_H
#define RENDER_H

#include "../include/cglm/cglm.h"

struct Scene;
struct Model;
struct Window;
struct ModelInstance;

void renderScene(struct Scene *scene, struct Window *window);
void renderModel(struct Model *model, mat4 viewMat, mat4 projectionMat);
void renderModelInstance(struct ModelInstance *instance, unsigned int shaderProgram);

#endif
