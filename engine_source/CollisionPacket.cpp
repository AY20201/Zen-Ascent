#include"../engine_headers/CollisionPacket.h"

//http://www.peroxide.dk/papers/collision/collision.pdf

typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool CollisionPacket::CheckPointInTriangle(glm::vec3 point, glm::vec3 pa, glm::vec3 pb, glm::vec3 pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;

	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);

	float ac_bb = (a * c) - (b * b);

	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}

void CollisionPacket::CheckTriangleCollision(glm::vec3 p1r3, glm::vec3 p2r3, glm::vec3 p3r3)
{
	glm::vec3 p1 = p1r3 / eRadius;
	glm::vec3 p2 = p2r3 / eRadius;
	glm::vec3 p3 = p3r3 / eRadius;

	CollisionPlane trianglePlane(p1, p2, p3);

	if (trianglePlane.IsFrontFacingTo(normalizedVelocity))
	{
		double t0, t1; //interval along velocity vector
		bool embeddedInPlane = false;

		double signedDistTrianglePlane = trianglePlane.SignedDistanceTo(basePoint);

		float normalDotVelocity = glm::dot(trianglePlane.normal, velocity);

		if (normalDotVelocity == 0.0f) //traveling parallel
		{
			if (fabs(signedDistTrianglePlane) >= 1.0f)
			{
				return; //not in plane
			}
			else
			{
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			t0 = (-1.0 - signedDistTrianglePlane) / normalDotVelocity;
			t1 = (-1.0 - signedDistTrianglePlane) / normalDotVelocity;

			if (t0 > t1)
			{
				double temp = t1;
				t1 = t0;
				t0 = temp;
			}

			if (t0 > 1.0f || t1 < 0.0f)
			{
				return; //no possible collision
			}

			t0 = fmax(t0, 0.0);
			t0 = fmin(t0, 1.0);
			t1 = fmax(t1, 0.0);
			t1 = fmin(t1, 1.0);
		}

		glm::vec3 collisionPoint;
		bool foundCollision = false;
		float t = 1.0f;

		//collision inside triangle
		if (!embeddedInPlane)
		{
			glm::vec3 planeIntersectionPoint = (basePoint - trianglePlane.normal) + (float)t0 * velocity;
			if (CheckPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				foundCollision = true;
				t = (float)t0;
				collisionPoint = planeIntersectionPoint;
			}
		}

		if (foundCollision == false)
		{
			float velLength = glm::length(velocity);
			float velocitySqrLength = velLength * velLength;
			float a, b, c;
			float newT = 0.0f;

			a = velocitySqrLength;
			//p1
			b = 2.0f * glm::dot(velocity, basePoint - p1);
			c = glm::length(p1 - basePoint) * glm::length(p1 - basePoint) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p1;
			}

			//p2
			b = 2.0f * glm::dot(velocity, basePoint - p2);
			c = glm::length(p2 - basePoint) * glm::length(p2 - basePoint) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p2;
			}

			//p3
			b = 2.0f * glm::dot(velocity, basePoint - p3);
			c = glm::length(p3 - basePoint) * glm::length(p3 - basePoint) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p3;
			}
			
			//edge1
			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - basePoint;
			float edgeSquaredLength = glm::length(edge) * glm::length(edge);
			float edgeDotVelocity = glm::dot(edge, velocity);
			float edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocitySqrLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - glm::length(baseToVertex) * glm::length(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p1 + f * edge;
				}
			}
			//edge2
			edge = p3 - p2;
			baseToVertex = p2 - basePoint;
			edgeSquaredLength = glm::length(edge) * glm::length(edge);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocitySqrLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - glm::length(baseToVertex) * glm::length(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p2 + f * edge;
				}
			}
			//edge3
			edge = p1 - p3;
			baseToVertex = p3 - basePoint;
			edgeSquaredLength = glm::length(edge) * glm::length(edge);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocitySqrLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - glm::length(baseToVertex) * glm::length(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestRoot(a, b, c, t, &newT))
			{
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}
		//set information
		if (foundCollision)
		{
			float distToCollision = t * glm::length(velocity);

			if (!CollisionPacket::foundCollision || distToCollision < nearestDistance)
			{
				CollisionPacket::nearestDistance = distToCollision;
				CollisionPacket::intersectionPoint = collisionPoint;
				CollisionPacket::foundCollision = true;
				CollisionPacket::time = t;
			}
		}
	}
}

bool CollisionPacket::GetLowestRoot(float a, float b, float c, float maxR, float* root)
{
	float determinant = b * b - 4.0f * a * c;

	if (determinant < 0.0f) return false;

	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);

	if (r1 > r2)
	{
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}

	if (r1 > 0.0f && r1 < maxR)
	{
		*root = r1;
		return true;
	}

	if (r2 > 0.0f && r2 < maxR)
	{
		*root = r2;
		return true;
	}

	return false;
}