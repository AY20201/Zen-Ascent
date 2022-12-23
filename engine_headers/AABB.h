#ifndef AABB_CLASS_H
#define AABB_CLASS_H

#include"Transform.h"
#include"Mesh.h"

class AABB
{
public:
	glm::vec3 extentsMax = glm::vec3(1.0f);
	glm::vec3 extentsMin = glm::vec3(1.0f);
	glm::vec3 center = glm::vec3(0.0f);

	glm::vec3 originalExtentsMax = glm::vec3(1.0f);
	glm::vec3 originalExtentsMin = glm::vec3(1.0f);
	glm::vec3 originalCenter = glm::vec3(0.0f);

	glm::mat4 lastTransformMatrix = glm::mat4(-10000.0f);
	bool isColliding = false;

	AABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix);
	AABB(glm::vec3 min, glm::vec3 max, glm::mat4 transformMatrix);
	AABB() = default;

	void RecalculateAABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix);
	void TransformExtents(glm::mat4 transformMatrix);
	bool CollideWithAABB(AABB& aabb);
	glm::vec3 GetNewVelocity(AABB& aabb, glm::vec3 currentVelocity, glm::vec3 gravity);
	glm::vec3 GetCollisionNormal(AABB& aabb);
};

#endif

