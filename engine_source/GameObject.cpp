#include"../engine_headers/GameObject.h"
#include"../engine_headers/Behavior.h"
#include"../engine_headers/ObjectHandler.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh, Behavior* behavior)
{
	Transform transform(position, rotation, scale);
	GameObject::transform = transform;
	meshes.push_back(mesh);
	GameObject::behavior = behavior;

	if (behavior != nullptr)
	{
		behavior->Initialize(this);
	}
	
	id = (int)ObjectHandler::Instance.gameObjects.size();
	ObjectHandler::Instance.AddGameObject(this);
}

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::vector<Mesh> meshes, Behavior* behavior)
{
	Transform transform(position, rotation, scale);
	GameObject::transform = transform;
	GameObject::meshes = meshes;
	GameObject::behavior = behavior;

	if (behavior != nullptr)
	{
		behavior->Initialize(this);
	}

	id = (int)ObjectHandler::Instance.gameObjects.size();
	ObjectHandler::Instance.AddGameObject(this);
}