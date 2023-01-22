#ifndef OBJECTHANDLER_CLASS_H
#define OBJECTHANDLER_CLASS_H

#include"GameObject.h"
#include"Behavior.h"
#include"Shader.h"
#include<vector>

class ObjectHandler 
{
	public:

		enum TransparentRender
		{
			SKIP_TRANSPARENCY,
			DRAW_TRANSPARENCY,
			DRAW_ONLY_TRANSPARENCY
		};

		static ObjectHandler Instance;
		std::vector <GameObject*> gameObjects;
		
		void AddGameObject(GameObject* gameObject);
		GameObject* GetGameObject(int id);
		void DrawMeshes(TransparentRender transparency);
		void DrawMeshes(Shader& shader, TransparentRender transparency);
		void Awake();
		void Update(float deltaTime, GLFWwindow* window);
		void Delete();

};

#endif