#include"../game_headers/PlayerController.h"

PlayerController::PlayerController(float movementSpeed, float height, Camera* camera, glm::vec3 boxDimensions) 
	: CollisionObject(boxDimensions, glm::vec3(0.0f, boxDimensions.y / 2.0f, 0.0f), glm::vec3(0.0f))
{
	PlayerController::movementSpeed = movementSpeed;
	PlayerController::height = height;
	PlayerController::camera = camera;

	timeSinceLastJump = 0.0f;
	timeLastGrounded = 0.0f;
	elaspedTime = 0.0f;
	collectablesPickedUp = 0;
	initialJumpVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void PlayerController::Awake()
{
	position = parentObj->transform.position;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	//camera->Position = parentObj->transform.position + glm::vec3(0.0f, height, 0.0f);
	camera->Position = position + glm::vec3(0.0f, height, 0.0f);
	camera->Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	
	ray.origin = camera->Position;
	ray.direction = camera->Orientation;

	initialJumpVelocity = glm::vec3(0.0f, sqrtf(2 * -gravity.y * jumpHeight), 0.0f);
}

void PlayerController::Update(float deltaTime, GLFWwindow* window)
{
	if (!escaped)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		bool wPressed = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		bool sPressed = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		bool aPressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

		float verticalAxis = float(wPressed) - float(sPressed);
		float horizontalAxis = float(dPressed) - float(aPressed);

		glm::vec3 flatOrientation = glm::vec3(camera->Orientation.x, 0.0f, camera->Orientation.z);
		glm::vec3 movementVec = verticalAxis * flatOrientation + horizontalAxis * glm::normalize(glm::cross(flatOrientation, camera->Up));
		if (movementVec != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			movementVec = glm::normalize(movementVec);
		}

		position = parentObj->transform.position;

		verticalMovementVector = verticalMovementVector + gravity * deltaTime;
		velocity = movementVec + verticalMovementVector;

		glm::vec3 newPosition = position + velocity * movementSpeed * deltaTime;

		glm::vec3 origExMin = boxCollider.extentsMin;
		glm::vec3 origExMax = boxCollider.extentsMax;

		glm::vec3 origTop = capsuleCollider.top;
		glm::vec3 origBottom = capsuleCollider.bottom;
		glm::vec3 origStart = capsuleCollider.start;
		glm::vec3 origEnd = capsuleCollider.end;

		boxCollider.TransformExtents(glm::translate(newPosition));
		capsuleCollider.TransformExtents(glm::translate(newPosition));

		//orthogonal vectors
		glm::vec3 ux = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 uy = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 uz = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::vec3 newVelocityVector = velocity;

		AABB zeroAABB = AABB(glm::vec3(0.0f), glm::vec3(0.0f), glm::mat4(0.0f));

		collisionRecursionDepth = 0;
		newVelocityVector = CollideWithWorld(newVelocityVector, movementSpeed, deltaTime, verticalMovementVector, zeroAABB);

		timeSinceLastJump += deltaTime;
		elaspedTime += deltaTime;

		isGrounded = newVelocityVector.y == 0.0f;

		//"ground check"
		if (isGrounded)
		{
			verticalMovementVector = glm::vec3(0.0f);

			if (timeLastGrounded != elaspedTime - deltaTime && elaspedTime - timeLastGrounded > landingDelay)
			{
				AudioPlayer::Instance.Play3DSound("../../engine_resource/Sound/soundeffects/landing.wav", parentObj->transform.position, 1.0f, 2.0f, false);
			}

			timeLastGrounded = elaspedTime;
		}
		//jump
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && timeSinceLastJump > jumpDelay && elaspedTime - timeLastGrounded < coyoteTime) //jump check
		{
			verticalMovementVector = initialJumpVelocity;
			timeSinceLastJump = 0.0f;

			//play audio
		}

		//clamp velocity
		if (verticalMovementVector.y > terminalVelocity.y)
		{
			verticalMovementVector = terminalVelocity;
		}

		velocity = newVelocityVector;

		//damping, only on x and z axis
		//velocity = velocity / glm::vec3((1.0f + damping * deltaTime), 1.0f, (1.0f + damping * deltaTime));

		position = position + newVelocityVector * movementSpeed * deltaTime;

		boxCollider.extentsMin = origExMin;
		boxCollider.extentsMax = origExMax;

		capsuleCollider.top = origTop;
		capsuleCollider.bottom = origBottom;
		capsuleCollider.start = origStart;
		capsuleCollider.end = origEnd;

		parentObj->transform.position = position;
		camera->Position = parentObj->transform.position + glm::vec3(0.0f, height, 0.0f);

		AudioPlayer::Instance.SetListenerPosition(camera->Position, camera->Orientation);

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS /*manual*/ || position.y < -25.0f /*kill floor*/) //reset position
		{
			if (!Transitioner::Instance.inTransitionStart && !Transitioner::Instance.inTransitionEnd)
			{
				Transitioner::Instance.SetTransition(0, true);
				Transitioner::Instance.inTransitionStart = true;
			}

			if (Transitioner::Instance.threshold == 2.0f)
			{
				parentObj->transform.position = glm::vec3(1.0f, 2.0f, 1.0f);
				velocity.y = 0.0f;
				verticalMovementVector = glm::vec3(0.0f);
				camera->Orientation = glm::vec3(0.74f, 0.1f, 0.66f);
			}
		}

		ray.origin = parentObj->transform.position + glm::vec3(0.0f, height / 2.0f, 0.0f);
		ray.direction = velocity;
		ray.maxDist = 0.5f;

		CollisionSolver::Instance.UpdateWorldCollisionsRay(ray);

		if (ray.hitObject != nullptr)
		{
			Collectable* collectable = dynamic_cast<Collectable*>(ray.hitObject->behavior);
			if (collectable != nullptr && !collectable->isCollected)
			{
				collectable->Collect();
				collectablesPickedUp += 1;
				//delete collectable;
			}
		}
		//std::cout << ray.collisionPoint.x << " " << ray.collisionPoint.y << " " << ray.collisionPoint.z << std::endl;

		ray.time = 0.0f;
		ray.foundCollision = false;
		ray.collisionPoint = glm::vec3(0.0f);
		ray.hitObject = nullptr;

		camera->Look(window);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		escaped = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && escaped == true)
	{
		escaped = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	
	parentObj->transform.RecalculateMatrix();
	boxCollider.TransformExtents(parentObj->transform.matrix);
}