#ifndef AABB_CLASS_H
#define AABB_CLASS_H

#include"Transform.h"
#include"Mesh.h"

class AABB
{
public:
	glm::vec3 extentsMax = glm::vec3(0.0f);
	glm::vec3 extentsMin = glm::vec3(0.0f);
	glm::mat4 lastTransformMatrix = glm::mat4(-10000.0f);
	bool isColliding;

	void RecalculateAABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix);
	void TransformExtents(glm::mat4 transformMatrix);
	bool CollideWithAABB(AABB& aabb);
};

#endif

