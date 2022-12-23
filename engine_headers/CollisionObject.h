#ifndef COLLISIONOBJECT_CLASS_H
#define COLLISIONOBJECT_CLASS_H

#include"CollisionPacket.h"
#include"AABB.h"

class CollisionObject
{
public:
	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec3 gravity = glm::vec3(0.0f, -1.4f, 0.0f);

	CollisionPacket* packet;
	AABB boxCollider;

	glm::vec3 colliderRadius;
	int collisionRecursionDepth = 0;
	int maxRecursionDepth = 5;

	//CollisionObject(glm::vec3 colliderRadius, glm::vec3 position, glm::vec3 velocity);
	CollisionObject(glm::vec3 boxDimensions, glm::vec3 position, glm::vec3 velocity);
	void UpdateCollisionPacket(glm::vec3 r3Pos, glm::vec3 r3Vel);

	glm::vec3 CollideWithWorld(glm::vec3 currentVelocity, float movementSpeed, float deltaTime, glm::vec3 gravity, AABB& lastCollision);

	//void CollideAndSlide(glm::vec3 r3Vel, glm::vec3 gravity, float movementSpeed, float deltaTime);
	//glm::vec3 CollideWithWorld(glm::vec3 ePos, glm::vec3 eVel, CollisionPlane lastPlane);
};

#endif
