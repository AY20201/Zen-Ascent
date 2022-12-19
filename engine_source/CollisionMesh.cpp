#include"../engine_headers/CollisionMesh.h"
#include"../engine_headers/CollisionSolver.h"
#include"../engine_headers/GameObject.h"

CollisionMesh::CollisionMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, glm::mat4 transformMatrix, GameObject* connectedGO)
{
	CollisionMesh::vertices = vertices;

	if (transformMatrix != glm::mat4(1.0f))
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			Vertex v = vertices[i];

			CollisionMesh::vertices[i].position = glm::vec3(glm::vec4(v.position.x, v.position.y, v.position.z, 1.0f) * transformMatrix);
			CollisionMesh::vertices[i].texcoord = glm::vec2(v.texcoord.x, v.texcoord.y);
			CollisionMesh::vertices[i].normal = glm::vec3(v.normal.x, v.normal.y, v.normal.z) * glm::mat3(glm::transpose(glm::inverse(transformMatrix)));
			CollisionMesh::vertices[i].tangent = glm::vec3(v.tangent.x, v.tangent.y, v.tangent.z);
		}
	}

	CollisionMesh::indices = indices;
	CollisionMesh::connectObject = connectedGO;

	boxCollider.RecalculateAABB(vertices, transformMatrix);

	CollisionSolver::Instance.sceneCollisionMeshes.push_back(*this);
}

void CollisionMesh::CheckAllTriangles(CollisionObject* collisionObject)
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];

		collisionObject->packet->CheckTriangleCollision(v0.position, v1.position, v2.position);
	}
}

void CollisionMesh::CheckRaycast(Raycast& ray)
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		if (!ray.foundCollision)
		{
			Vertex& v0 = vertices[indices[i]];
			Vertex& v1 = vertices[indices[i + 1]];
			Vertex& v2 = vertices[indices[i + 2]];

			ray.CheckRayTriangleCollision(10.0f, v0.position, v1.position, v2.position);
			if (ray.foundCollision)
			{
				ray.hitObject = connectObject;
			}
			else
			{
				ray.hitObject = nullptr;
			}
		}
	}
}