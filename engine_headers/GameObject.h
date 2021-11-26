#ifndef GAMEOBJECT_CLASS_H
#define GAMEOBJECT_CLASS_H

#include"Transform.h"
#include"Mesh.h"

class GameObject
{
	public:

		Transform transform;
		Mesh mesh;

		GameObject(Transform transform, Mesh mesh);
		static GameObject Instantiate(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh);

};

#endif

