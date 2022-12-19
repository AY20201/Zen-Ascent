#ifndef PLAYERCONTROLLER_CLASS_H
#define PLAYERCONTROLLER_CLASS_H

#include"../engine_headers/GameObject.h"
#include"../engine_headers/Behavior.h"
#include"../engine_headers/CollisionObject.h"
#include"../engine_headers/CollisionSolver.h"
#include"../engine_headers/Raycast.h"
#include"../engine_headers/Camera.h"

class PlayerController : public Behavior, public CollisionObject
{
public:

	float movementSpeed = 1.0f;
	float height = 0.5f;
	Camera* camera;
	bool escaped = false;

	Raycast ray;
	//glm::vec3 startingPosition;

	PlayerController(float movementSpeed, float height, Camera* camera, glm::vec3 boxDimensions);
	void Awake() override;
	void Update(float deltaTime, GLFWwindow* window) override;
};

#endif

