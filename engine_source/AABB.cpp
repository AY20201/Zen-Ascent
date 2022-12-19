#include"../engine_headers/AABB.h"

void AABB::RecalculateAABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix)
{
	if (transformMatrix != lastTransformMatrix)
	{
		glm::vec3 tExtentsMax = glm::vec3(-10000.0f);
		glm::vec3 tExtentsMin = glm::vec3(10000.0f);
		for (int i = 0; i < vertices.size(); i++)
		{
			glm::vec3 vPos = vertices[i].position;

			if (vPos.x > tExtentsMax.x && vPos.y > tExtentsMax.y && vPos.z > tExtentsMax.z)
			{
				tExtentsMax = vPos;
				extentsMax = tExtentsMax;
			}
			if (vPos.x < tExtentsMin.x && vPos.y < tExtentsMin.y && vPos.z < tExtentsMin.z)
			{
				tExtentsMin = vPos;
				extentsMin = tExtentsMin;
			}
		}

		extentsMax = glm::vec3(glm::vec4(extentsMax, 0.0f) * transformMatrix);
		extentsMin = glm::vec3(glm::vec4(extentsMin, 0.0f) * transformMatrix);

		lastTransformMatrix = transformMatrix;
	}
}

void AABB::TransformExtents(glm::mat4 transformMatrix)
{
	extentsMax = glm::vec3(glm::vec4(extentsMax, 0.0f) * transformMatrix);
	extentsMin = glm::vec3(glm::vec4(extentsMin, 0.0f) * transformMatrix);
}

bool AABB::CollideWithAABB(AABB& aabb)
{
	glm::vec3 minA = extentsMin;
	glm::vec3 minB = aabb.extentsMin;
	glm::vec3 maxA = extentsMax;
	glm::vec3 maxB = aabb.extentsMax;

	return (
		minA.x <= maxB.x &&
		maxA.x >= minB.x &&
		minA.y <= maxB.y &&
		maxA.y >= minB.y &&
		minA.z <= maxB.z &&
		maxA.z >= minB.z
	);
}