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
		if (CollisionSolver::Instance.sceneCollisionMeshes[i].hasBoxCollider)
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
	}
	
	return currentVelocity;
}