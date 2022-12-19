#ifndef COLLISIONPACKET_CLASS_H
#define COLLISIONPACKET_CLASS_H

#include"Transform.h"
#include"CollisionPlane.h"
#include<iostream>

class CollisionPacket
{
public:
	glm::vec3 eRadius; //ellipsoid radius

	glm::vec3 r3Velocity; //in r3 space
	glm::vec3 r3Position;

	glm::vec3 velocity; //ellipsoid space
	glm::vec3 normalizedVelocity;
	glm::vec3 basePoint;

	bool foundCollision; //collision information
	double nearestDistance;
	glm::vec3 intersectionPoint;
	float time;

	bool CheckPointInTriangle(glm::vec3 point, glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);
	void CheckTriangleCollision(glm::vec3 p1r3, glm::vec3 p2r3, glm::vec3 p3r3);
private:
	bool GetLowestRoot(float a, float b, float c, float maxR, float* root);
};

#endif

