#include"../engine_headers/GameObject.h"
#include"../engine_headers/ObjectHandler.h"

GameObject::GameObject(Transform transform, Mesh mesh) 
{
	GameObject::transform = transform;
	GameObject::mesh = mesh;
}

GameObject GameObject::Instantiate(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Mesh mesh)
{
	Transform transform(position, rotation, scale);
	GameObject newObject(transform, mesh);
	ObjectHandler::Instance.AddGameObject(&newObject);
	return newObject;
}