#ifndef COLLISIONMESH_CLASS_H
#define COLLISIONMESH_CLASS_H

#include"CollisionObject.h"
#include"Raycast.h"
#include"Mesh.h"
#include"AABB.h"

class GameObject;

class CollisionMesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	bool hasBoxCollider;

	AABB boxCollider;
	GameObject* connectObject;

	CollisionMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, glm::mat4 transformMatrix, GameObject* connectedGO, bool hasBoxCollider);
	void CheckRaycast(Raycast& ray);
};

#endif
