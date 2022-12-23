#include"../engine_headers/AABB.h"

AABB::AABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix)
{
	RecalculateAABB(vertices, transformMatrix);
	originalCenter = (originalExtentsMax + originalExtentsMin) / 2.0f;
	TransformExtents(transformMatrix);
}

AABB::AABB(glm::vec3 min, glm::vec3 max, glm::mat4 transformMatrix)
{
	originalExtentsMax = max;
	originalExtentsMin = min;
	originalCenter = (originalExtentsMax + originalExtentsMin) / 2.0f;

	TransformExtents(transformMatrix);
}

void AABB::RecalculateAABB(std::vector<Vertex>& vertices, glm::mat4 transformMatrix)
{
	if (transformMatrix != lastTransformMatrix)
	{
		glm::vec3 tExtentsMax = glm::vec3(-10000.0f);
		glm::vec3 tExtentsMin = glm::vec3(10000.0f);
		for (int i = 0; i < vertices.size(); i++)
		{
			glm::vec3 vPos = vertices[i].position;
			if (vPos.x < tExtentsMin.x)
			{
				tExtentsMin.x = vPos.x;
			}
			if (vPos.y < tExtentsMin.y)
			{
				tExtentsMin.y = vPos.y;
			}
			if (vPos.z < tExtentsMin.z)
			{
				tExtentsMin.z = vPos.z;
			}
			if (vPos.x > tExtentsMax.x)
			{
				tExtentsMax.x = vPos.x;
			}
			if (vPos.y > tExtentsMax.y)
			{
				tExtentsMax.y = vPos.y;
			}
			if (vPos.z > tExtentsMax.z)
			{
				tExtentsMax.z = vPos.z;
			}
		}

		//if it is a plane, use different conversion test
		if (tExtentsMax.x == tExtentsMin.x) tExtentsMin.x = tExtentsMin.x - 0.1f;
		if (tExtentsMax.y == tExtentsMin.y) tExtentsMin.y = tExtentsMin.y - 0.1f;
		if (tExtentsMax.z == tExtentsMin.z) tExtentsMin.z = tExtentsMin.z - 0.1f;

		extentsMax = tExtentsMax;
		extentsMin = tExtentsMin;

		originalExtentsMax = extentsMax;
		originalExtentsMin = extentsMin;
		TransformExtents(transformMatrix);

		lastTransformMatrix = transformMatrix;
	}
}

void AABB::TransformExtents(glm::mat4 transformMatrix)
{
	extentsMax = glm::vec3(transformMatrix * glm::vec4(originalExtentsMax, 1.0f));
	extentsMin = glm::vec3(transformMatrix * glm::vec4(originalExtentsMin, 1.0f));
	center = glm::vec3(transformMatrix * glm::vec4(originalCenter, 1.0f));
}

bool AABB::CollideWithAABB(AABB& aabb)
{
	glm::vec3 minA = extentsMin;
	glm::vec3 minB = aabb.extentsMin;
	glm::vec3 maxA = extentsMax;
	glm::vec3 maxB = aabb.extentsMax;

	if (minB.y == maxB.y) //if it is a plane
	{
		glm::vec3 c = center; //center
		glm::vec3 e = maxA - c; //extents

		//plane normal, always up
		glm::vec3 pn = glm::vec3(0.0f, 1.0f, 0.0f);

		float r = e.x * pn.x + e.y * pn.y + e.z * pn.z;
		float d = -(pn.x * c.x + pn.y * c.y + pn.z * c.z); //plane d

		float signedDist = glm::dot(pn, c) - d;

		std::cout << (fabs(signedDist) <= r) << std::endl;

		return fabs(signedDist) <= r && (
			minA.x <= maxB.x &&
			maxA.x >= minB.x &&
			minA.z <= maxB.z &&
			maxA.z >= minB.z
		);
	}

	return (
		minA.x <= maxB.x &&
		maxA.x >= minB.x &&
		minA.y <= maxB.y &&
		maxA.y >= minB.y &&
		minA.z <= maxB.z &&
		maxA.z >= minB.z
	);
}

glm::vec3 AABB::GetNewVelocity(AABB& aabb, glm::vec3 currentVelocity, glm::vec3 gravity)
{
	/*
	glm::vec3 ux = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 uy = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 uz = glm::vec3(0.0f, 0.0f, 1.0f);

	//https://gamedev.stackexchange.com/questions/136073/how-does-one-calculate-the-surface-normal-in-2d-collisions
	glm::vec3 e = aabb.extentsMax - aabb.center;

	glm::vec3 d = center - aabb.center;

	float dx = glm::dot(d, ux);
	float dy = glm::dot(d, uy);
	float dz = glm::dot(d, uz);

	//float adx = fabs(dx);
	//float ady = fabs(dy);
	//float adz = fabs(dz);

	if (dx > e.x) dx = e.x;
	if (dx < -e.x) dx = -e.x;

	if (dy > e.y) dy = e.y;
	if (dy < -e.y) dy = -e.y;

	if (dz > e.z) dz = e.z;
	if (dz < -e.z) dz = -e.z;

	glm::vec3 p = aabb.center + dx * ux + dy * uy + dz * uz;

	//prevents clipping out of world due to divide by zero error
	glm::vec3 collisionNormal = center - p;
	if (collisionNormal != glm::vec3(0.0f))
	{
		collisionNormal = glm::normalize(collisionNormal);
	}

	collisionNormal = glm::vec3(roundf(collisionNormal.x), roundf(collisionNormal.y), roundf(collisionNormal.z));
	*/
	
	glm::vec3 collisionNormal = GetCollisionNormal(aabb);

	if (collisionNormal.y == 0.0f)
	{
		glm::vec3 perpendicularVec = glm::vec3(collisionNormal.z, 0.0f, -collisionNormal.x);
		float scalar = glm::dot(perpendicularVec, currentVelocity - gravity);

		return (perpendicularVec * scalar) + gravity;
	}
	else
	{
		return currentVelocity - gravity;
	}
	
}

glm::vec3 AABB::GetCollisionNormal(AABB& aabb)
{
	//https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/4collisiondetection/Physics%20-%20Collision%20Detection.pdf
	glm::vec3 faces[6]
	{
		glm::vec3( 1.0f, 0.0f, 0.0f), 
		glm::vec3( -1.0f, 0.0f, 0.0f),
		glm::vec3( 0.0f, 1.0f, 0.0f), 
		glm::vec3( 0.0f, -1.0f , 0.0f),
		glm::vec3( 0.0f, 0.0f, 1.0f), 
		glm::vec3( 0.0f, 0.0f, -1.0f),
	};

	float distances[6]
	{
		(aabb.extentsMax.x - extentsMin.x),
		(extentsMax.x - aabb.extentsMin.x),
		(aabb.extentsMax.y - extentsMin.y),
		(extentsMax.y - aabb.extentsMin.y),
		(aabb.extentsMax.z - extentsMin.z),
		(extentsMax.z - aabb.extentsMin.z)
	};

	float penetration = 10000.0f;
	glm::vec3 bestAxis = glm::vec3(0.0f);

	for (int i = 0; i < 6; i++)
	{
		if (distances[i] < penetration)
		{
			penetration = distances[i];
			bestAxis = faces[i];
		}
	}

	return bestAxis;
}