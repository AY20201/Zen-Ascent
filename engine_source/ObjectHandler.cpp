#include"../engine_headers/ObjectHandler.h"

ObjectHandler ObjectHandler::Instance;

void ObjectHandler::AddGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

void ObjectHandler::DrawMeshes()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->mesh.Draw(gameObjects[i]->transform.matrix);
	}
}

void ObjectHandler::DrawMeshes(Shader& shader)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->mesh.Draw(gameObjects[i]->transform.matrix, shader);
	}
}

