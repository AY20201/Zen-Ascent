#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include"Transform.h"
#include"Mesh.h"
#include<vector>

class Behavior;

class GameObject
{
	public:

		Transform transform;
		std::vector<Mesh> meshes;
		Behavior* behavior;

		int id;

		GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh, Behavior* behavior);
		GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::vector<Mesh> meshes, Behavior* behavior);

};

#endif

