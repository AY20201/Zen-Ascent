#ifndef PLAYERCONTROLLER_CLASS_H
#define PLAYERCONTROLLER_CLASS_H

#include"../engine_headers/GameObject.h"
#include"../engine_headers/Behavior.h"
#include"../engine_headers/CollisionObject.h"
#include"../engine_headers/CollisionSolver.h"
#include"../engine_headers/Raycast.h"
#include"../engine_headers/Camera.h"
#include"../engine_headers/AudioPlayer.h"
#include"../engine_headers/Transitioner.h"
#include"Collectable.h"

class PlayerController : public Behavior, public CollisionObject
{
public:

	float movementSpeed = 1.0f;
	float height = 0.5f;
	Camera* camera;
	bool escaped = false;

	glm::vec3 gravity = glm::vec3(0.0f, -6.0f, 0.0f);
	glm::vec3 verticalMovementVector = glm::vec3(0.0f);
	glm::vec3 terminalVelocity = glm::vec3(0.0f, 4.0f, 0.0f);

	float jumpHeight = 0.25f;
	bool isGrounded = false;

	float jumpDelay = 0.5f;
	float landingDelay = 0.25f;
	float timeSinceLastJump;

	float coyoteTime = 0.2f;
	float timeLastGrounded;
	float elaspedTime;

	glm::vec3 initialJumpVelocity;

	float damping = 1.0f;

	Raycast ray;
	//glm::vec3 startingPosition;

	PlayerController(float movementSpeed, float height, Camera* camera, glm::vec3 boxDimensions);
	void Awake() override;
	void Update(float deltaTime, GLFWwindow* window) override;
};

#endif

