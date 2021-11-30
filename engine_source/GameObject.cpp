#include"../engine_headers/GameObject.h"
#include"../engine_headers/ObjectHandler.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh)
{
	Transform transform(position, rotation, scale);
	GameObject::transform = transform;
	GameObject::mesh = mesh;

	ObjectHandler::Instance.AddGameObject(this);
}