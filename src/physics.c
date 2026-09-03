
#include "physics.h"

void initBody(struct Body *body, vec3 position, vec3 velocity, float mass, float radius) {
	glm_vec3_copy(position, body->position);
	glm_vec3_copy(velocity, body->velocity);
	body->mass = mass;
	body->radius = radius;
}

void calculateForce(struct Body *bodyA, struct Body *bodyB, vec3 totalForce) {
	vec3 distance;
	glm_vec3_sub(bodyB->position, bodyA->position, distance);
	double dist = glm_vec3_distance(bodyA->position, bodyB->position);

	if (dist <= 0.001)
		dist = 0.001;

	float force = 1.0 * bodyA->mass * bodyB->mass / pow(dist, 2);

	if (dist < bodyA->radius + bodyB->radius)
		force *= -0.1;

	vec3 u;
	glm_vec3_scale(distance, force / dist, u);
	glm_vec3_add(totalForce, u, totalForce);
}

void updatePlanets(struct Body *bodies, const unsigned int bodyCount, const float deltaTime) {
	vec3 velocity;
	vec3 acceleration;
	vec3 totalForce;

	for(int i = 0; i < bodyCount; i++) {
		glm_vec3_zero(totalForce);

		for(int j = 0; j < bodyCount; j++) {
			if(i == j)
				continue;

			calculateForce(&bodies[i], &bodies[j], totalForce);
		}

		glm_vec3_copy(totalForce, acceleration);
		glm_vec3_scale(acceleration, deltaTime / bodies[i].mass, acceleration);
		glm_vec3_add(bodies[i].velocity, acceleration, bodies[i].velocity);
	}

	for(int i = 0; i < bodyCount; i++) {
		glm_vec3_scale(bodies[i].velocity, deltaTime, velocity);
		glm_vec3_add(bodies[i].position, velocity, bodies[i].position);
	}

} 

