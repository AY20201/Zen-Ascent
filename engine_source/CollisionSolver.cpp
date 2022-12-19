#include"../engine_headers/CollisionSolver.h"

CollisionSolver CollisionSolver::Instance;


void CollisionSolver::UpdateWorldCollisions(CollisionObject* colObject)
{
	for (unsigned int i = 0; i < sceneCollisionMeshes.size(); i++)
	{
		sceneCollisionMeshes[i].CheckAllTriangles(colObject);
	}
}

void CollisionSolver::UpdateWorldCollisionsRay(Raycast& ray)
{
	for (unsigned int i = 0; i < sceneCollisionMeshes.size(); i++)
	{
		sceneCollisionMeshes[i].CheckRaycast(ray);
	}
}