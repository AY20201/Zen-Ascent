#ifndef OBJECTHANDLER_CLASS_H
#define OBJECTHANDLER_CLASS_H

#include"GameObject.h"
#include"Behavior.h"
#include"Shader.h"
#include<vector>

class ObjectHandler 
{
	public:

		static ObjectHandler Instance;
		std::vector <GameObject*> gameObjects;
		
		void AddGameObject(GameObject* gameObject);
		GameObject* GetGameObject(int id);
		void DrawMeshes();
		void DrawMeshes(Shader& shader);
		void Awake();
		void Update(float deltaTime, GLFWwindow* window);
		void Delete();

};

#endif