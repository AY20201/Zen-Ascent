#ifndef RAYCAST_CLASS_H
#define RAYCAST_CLASS_H

#include"Transform.h"
#include"CollisionPlane.h"
#include"GameObject.h"

class Raycast
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

	bool foundCollision;
	glm::vec3 collisionPoint;
	float time;
	GameObject* hitObject;

	void CheckRayTriangleCollision(float maxDist, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};

#endif
