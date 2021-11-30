#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include"Transform.h"
#include"Mesh.h"

class GameObject
{
	public:

		Transform transform;
		Mesh mesh;

		GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh);

};

#endif

