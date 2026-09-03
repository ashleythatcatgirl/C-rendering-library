
#ifndef PHYSICS_H
#define PHYSICS_H

#include "../include/cglm/cglm.h"

struct Body {
	vec3 position;
	vec3 velocity;

	float radius;
	float mass;
};

void initBody(struct Body *body, vec3 position, vec3 velocity, float mass, float radius);
void updatePlanets(struct Body *bodies, const unsigned int bodyCount, const float deltaTime);
void calculateForce(struct Body *bodyA, struct Body *bodyB, vec3 totalForce);

#endif
