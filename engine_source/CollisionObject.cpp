#include"../engine_headers/CollisionObject.h"
#include"../engine_headers/CollisionSolver.h"
/*
CollisionObject::CollisionObject(glm::vec3 colliderRadius, glm::vec3 position, glm::vec3 velocity)
{
	CollisionObject::position = position;
	CollisionObject::velocity = velocity;

	CollisionObject::colliderRadius = colliderRadius;

	packet = new CollisionPacket();

	packet->eRadius = colliderRadius;

	UpdateCollisionPacket(position, velocity);
}
*/

CollisionObject::CollisionObject(glm::vec3 boxDimensions, glm::vec3 position, glm::vec3 velocity)
{
	CollisionObject::position = position;
	CollisionObject::velocity = velocity;

	boxCollider = AABB(position - boxDimensions / 2.0f, position + boxDimensions / 2.0f, glm::mat4(1.0f));
	capsuleCollider = Capsule(position + glm::vec3(0.0f, boxDimensions.y / 2.0f, 0.0f), position - glm::vec3(0.0f, boxDimensions.y / 2.0f, 0.0f), boxDimensions.x / 2.0f);
}

/*
void CollisionObject::UpdateCollisionPacket(glm::vec3 r3Pos, glm::vec3 r3Vel)
{
	packet->r3Position = r3Pos;
	packet->r3Velocity = r3Vel;

	packet->basePoint = r3Pos / colliderRadius;
	packet->velocity = r3Vel / colliderRadius;
	packet->normalizedVelocity = glm::normalize(r3Vel / colliderRadius);

	//packet->CheckTriangleCollision(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.5f, 0.0f, 0.5f));
	//std::cout << packet->velocity.x << " " << packet->velocity.y << " " << packet->velocity.z << " " << std::endl;
}
*/

glm::vec3 CollisionObject::CollideWithWorld(glm::vec3 currentVelocity, float movementSpeed, float deltaTime, glm::vec3 vmv, AABB& lastCollision)
{
	//vmv = verticalMovementVector
	glm::vec3 newVelocityVector = currentVelocity;

	if (collisionRecursionDepth > maxRecursionDepth)
	{
		return currentVelocity;
	}

	collisionRecursionDepth += 1;

	for (unsigned int i = 0; i < CollisionSolver::Instance.sceneCollisionMeshes.size(); i++)
	{
		AABB aabb = CollisionSolver::Instance.sceneCollisionMeshes[i].boxCollider;

		if (lastCollision.extentsMax != aabb.extentsMax && lastCollision.extentsMin != aabb.extentsMin)
		{
			if (boxCollider.CollideWithAABB(aabb))
			{
				newVelocityVector = boxCollider.GetNewVelocity(aabb, currentVelocity, vmv);

				glm::vec3 newPosition = position + newVelocityVector * movementSpeed * deltaTime;

				boxCollider.TransformExtents(glm::translate(glm::mat4(1.0f), newPosition));
				capsuleCollider.TransformExtents(glm::translate(glm::mat4(1.0f), newPosition));

				return CollideWithWorld(newVelocityVector, movementSpeed, deltaTime, vmv, aabb);
			}
		}
	}
	
	return currentVelocity;
}

/*
void CollisionObject::CollideAndSlide(glm::vec3 r3Vel, glm::vec3 gravity, float movementSpeed, float deltaTime)
{
	packet->r3Position = position;
	packet->r3Velocity = r3Vel;

	glm::vec3 eSpacePosition = packet->r3Position / colliderRadius;
	glm::vec3 eSpaceVelocity = packet->r3Velocity / colliderRadius;

	CollisionPlane firstPlane(glm::vec3(0.0f), glm::vec3(0.0f));

	collisionRecursionDepth = 0;
	glm::vec3 finalPosition = CollideWithWorld(eSpacePosition, eSpaceVelocity, firstPlane);
	
	packet->r3Position = finalPosition * colliderRadius;
	/*
	packet->r3Velocity = gravity;

	eSpaceVelocity = gravity / colliderRadius;

	collisionRecursionDepth = 0;
	firstPlane = CollisionPlane(glm::vec3(0.0f), glm::vec3(0.0f));
	finalPosition = CollideWithWorld(finalPosition, eSpaceVelocity, firstPlane);
	
	finalPosition = finalPosition * colliderRadius;

	glm::vec3 collisionMovement = finalPosition - position;
	glm::vec3 combinedVelocity = velocity + collisionMovement;
	float gravityMagnitude = glm::length(gravity);
	if (combinedVelocity != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		combinedVelocity = glm::normalize(combinedVelocity);
	}
	velocity = combinedVelocity * glm::vec3(movementSpeed, 1.0f, movementSpeed);
	position = position + velocity * deltaTime;
}
*/
/*
//original recursive
glm::vec3 CollisionObject::CollideWithWorld(glm::vec3 ePos, glm::vec3 eVel, CollisionPlane lastPlane)
{
	float closeDistance = 0.005f;

	if (collisionRecursionDepth > 5)
	{
		return ePos;
	}

	packet->velocity = eVel;
	packet->normalizedVelocity = glm::normalize(eVel);
	packet->basePoint = ePos;
	packet->foundCollision = false;

	CollisionSolver::Instance.UpdateWorldCollisions(this);

	if (packet->foundCollision == false)
	{
		return ePos + eVel;
	}

	glm::vec3 destPoint = ePos + eVel;
	/*
	glm::vec3 newBasePoint = ePos;

	if (packet->nearestDistance >= closeDistance)
	{
		glm::vec3 v = eVel;
		v = v / glm::length(v) * (float)(packet->nearestDistance - closeDistance);
		newBasePoint = packet->basePoint + v;
		v = glm::normalize(v);
		packet->intersectionPoint -= closeDistance * v;
	}

	glm::vec3 touchPoint = ePos + eVel * packet->time;
	float thresholdDistance = fmax(glm::length(velocity) * packet->time - closeDistance, 0.0f);
	glm::vec3 nearPoint = ePos + glm::normalize(eVel) * thresholdDistance;

	glm::vec3 slidePlaneOrigin = packet->intersectionPoint;
	glm::vec3 slidePlaneNormal = glm::normalize(touchPoint - packet->intersectionPoint);

	CollisionPlane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	float longRadius = 1.0f + closeDistance;
	glm::vec3 newDestinationPoint = destPoint - ((float)slidingPlane.SignedDistanceTo(destPoint) - longRadius) * slidePlaneNormal;
	glm::vec3 newVelocityVector = newDestinationPoint - packet->intersectionPoint;

	if (glm::length(newVelocityVector) < closeDistance)
	{
		return nearPoint;
	}

	collisionRecursionDepth++;
	return CollideWithWorld(nearPoint, newVelocityVector, lastPlane);
}
*/
//revised recursive
/*
glm::vec3 CollisionObject::CollideWithWorld(glm::vec3 ePos, glm::vec3 eVel, CollisionPlane lastPlane)
{
	float closeDistance = 0.001f;

	if (collisionRecursionDepth > 3)
	{
		return ePos;
	}

	packet->velocity = eVel;
	packet->normalizedVelocity = glm::normalize(eVel);
	packet->basePoint = ePos;
	packet->foundCollision = false;

	CollisionSolver::Instance.UpdateWorldCollisions(this);

	if (!packet->foundCollision)
	{
		return ePos + eVel;
	}

	glm::vec3 destPoint = ePos + eVel;

	glm::vec3 touchPoint = ePos + eVel * packet->time;
	float thresholdDistance = fmax(glm::length(velocity) * packet->time - closeDistance, 0.0f);
	glm::vec3 nearPoint = ePos + glm::normalize(eVel) * thresholdDistance;

	glm::vec3 slidePlaneOrigin = packet->intersectionPoint;
	glm::vec3 slidePlaneNormal = glm::normalize(touchPoint - packet->intersectionPoint);

	glm::vec3 newVelocityVector = glm::vec3(0.0f, 0.0f, 0.0f);

	if (collisionRecursionDepth == 0)
	{
		lastPlane = CollisionPlane(slidePlaneOrigin, slidePlaneNormal);

		float longRadius = 0.0f;//1.0f + closeDistance;
		glm::vec3 newDestinationPoint = destPoint - ((float)lastPlane.SignedDistanceTo(destPoint) - longRadius) * slidePlaneNormal;
		newVelocityVector = newDestinationPoint - packet->intersectionPoint;
	}
	else if (collisionRecursionDepth == 1)
	{
		CollisionPlane secondPlane = CollisionPlane(slidePlaneOrigin, slidePlaneNormal);
		glm::vec3 crease = glm::normalize(glm::cross(lastPlane.normal, secondPlane.normal));
		float dis = glm::dot(destPoint - ePos, crease);
		newVelocityVector = dis * crease;
	}
	
	if (glm::length(newVelocityVector) < closeDistance)
	{
		return nearPoint;
	}

	collisionRecursionDepth++;
	return CollideWithWorld(nearPoint, newVelocityVector, lastPlane);
}
*/

/*
glm::vec3 CollisionObject::CollideWithWorld(glm::vec3 ePos, glm::vec3 eVel)
{
	float closeDistance = 0.005f;

	glm::vec3 destPoint = ePos + eVel;

	CollisionPlane firstPlane;

	for (int i = 0; i < 3; i++)
	{
		packet->velocity = eVel;
		packet->normalizedVelocity = glm::normalize(eVel);
		packet->basePoint = ePos;
		packet->foundCollision = false;

		CollisionSolver::Instance.UpdateWorldCollisions(this);

		if (!packet->foundCollision)
		{
			return destPoint;
		}

		float dist = glm::length(eVel) * packet->time;
		float shortDist = fmax(dist - closeDistance, 0.0f);

		glm::vec3 touchPoint = ePos + eVel * packet->time;
		glm::vec3 nearPoint = ePos + glm::normalize(eVel) * shortDist;

		glm::vec3 slidePlaneOrigin = packet->intersectionPoint;
		glm::vec3 slidePlaneNormal = glm::normalize(touchPoint - packet->intersectionPoint);

		ePos = nearPoint;
		if (i == 0)
		{
			firstPlane = CollisionPlane(slidePlaneOrigin, slidePlaneNormal);
			float longRadius = 1.0f + closeDistance;
			destPoint -= ((float)firstPlane.SignedDistanceTo(destPoint) - longRadius) * firstPlane.normal;
			eVel = destPoint - ePos;
		}
		else if (i == 1)
		{
			CollisionPlane secondPlane(slidePlaneOrigin, slidePlaneNormal);
			glm::vec3 crease = glm::normalize(glm::cross(firstPlane.normal, secondPlane.normal));
			float dis = glm::dot((destPoint - ePos), crease);
			eVel = dis * crease;
			destPoint = ePos + eVel;
		}
	}

	return ePos;
}
*/
