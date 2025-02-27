#include"../engine_headers/ObjectHandler.h"

ObjectHandler ObjectHandler::Instance;

void ObjectHandler::AddGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

GameObject* ObjectHandler::GetGameObject(int id)
{
	return gameObjects[id];
}

void ObjectHandler::DrawMeshes(TransparentRender transparency) //0 = skip transparent, 1 = draw both, 2 = only transparent
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		for (unsigned int j = 0; j < gameObjects[i]->meshes.size(); j++)
		{
			if (transparency == DRAW_TRANSPARENCY)
			{
				gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix);
			}
			else if(transparency == SKIP_TRANSPARENCY)
			{
				if (!gameObjects[i]->meshes[j].material->transparent)
				{
					gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix);
				}
			}
			else if(transparency == DRAW_ONLY_TRANSPARENCY)
			{
				if (gameObjects[i]->meshes[j].material->transparent)
				{
					gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix);
				}
			}
			
		}
		//gameObjects[i]->mesh.Draw(gameObjects[i]->transform.matrix);
	}
}

void ObjectHandler::DrawMeshes(Shader& shader, TransparentRender transparency)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		for (unsigned int j = 0; j < gameObjects[i]->meshes.size(); j++)
		{
			if (transparency == DRAW_TRANSPARENCY)
			{
				gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix, shader);
			}
			else if (transparency == SKIP_TRANSPARENCY)
			{
				if (!gameObjects[i]->meshes[j].material->transparent)
				{
					gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix, shader);
				}
			}
			else if (transparency == DRAW_ONLY_TRANSPARENCY)
			{
				if (gameObjects[i]->meshes[j].material->transparent)
				{
					gameObjects[i]->meshes[j].Draw(gameObjects[i]->transform.matrix, shader);
				}
			}
		}
		//gameObjects[i]->mesh.Draw(gameObjects[i]->transform.matrix, shader);
	}
}

void ObjectHandler::Awake()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->behavior != nullptr)
		{
			gameObjects[i]->behavior->Awake();
		}
	}
}

void ObjectHandler::Update(float deltaTime, GLFWwindow* window)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->behavior != nullptr)
		{
			gameObjects[i]->behavior->Update(deltaTime, window);
		}
	}
}

void ObjectHandler::Delete()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->behavior != nullptr)
		{
			delete gameObjects[i]->behavior;
		}
		delete gameObjects[i];
	}
}


