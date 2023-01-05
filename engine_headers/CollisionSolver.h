#ifndef COLLISIONSOLVER_CLASS_H
#define COLLISIONSOLVER_CLASS_H

#include"CollisionObject.h"
#include"CollisionMesh.h"
#include<vector>

class CollisionSolver
{
public:
	static CollisionSolver Instance;

	std::vector<CollisionMesh> sceneCollisionMeshes;

	void UpdateWorldCollisionsRay(Raycast& ray);
};

#endif
