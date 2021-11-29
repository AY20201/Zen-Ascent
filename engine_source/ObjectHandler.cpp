#include"../engine_headers/ObjectHandler.h"

ObjectHandler ObjectHandler::Instance;

void ObjectHandler::AddGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

void ObjectHandler::DrawMeshes(Shader& shader)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->mesh.Draw(shader, gameObjects[i]->transform.matrix);
	}
}

