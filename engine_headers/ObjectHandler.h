#ifndef MESHHANDLER_CLASS_H
#define MESHHANDLER_CLASS_H

#include "GameObject.h"
#include "Shader.h"
#include <vector>

class ObjectHandler 
{
	public:

		static ObjectHandler Instance;
		std::vector <GameObject*> gameObjects;
		
		void AddGameObject(GameObject* gameObject);
		void DrawMeshes(Shader& shader);

};

#endif