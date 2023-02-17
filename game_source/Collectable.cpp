#include"../game_headers/Collectable.h"

Collectable::Collectable(bool isCollected, float rotateSpeed, float scaleDecreaseSpeed, float minScale)
{
	Collectable::isCollected = isCollected;
	Collectable::rotateSpeed = rotateSpeed;
	Collectable::scaleDecreaseSpeed = scaleDecreaseSpeed;
	Collectable::minScale = minScale;

	originalPosition = glm::vec3(0.0f);

	hoverScale = 0.05f;
	hoverSpeed = 0.5f;
}

void Collectable::Awake()
{
	isVisible = !isCollected;

	originalPosition = parentObj->transform.position;
	parentObj->transform.rotation.y = (float)(rand() % 360);
}

void Collectable::Update(float deltaTime, GLFWwindow* window)
{
	parentObj->transform.rotation.y = fmod(parentObj->transform.rotation.y + rotateSpeed * deltaTime, 360.0f);

	parentObj->transform.position.y = originalPosition.y + sinf((float)glfwGetTime() * hoverSpeed) * hoverScale;

	if (isVisible)
	{
		if (isCollected)
		{
			parentObj->transform.scale -= glm::vec3(scaleDecreaseSpeed * deltaTime);
			if (parentObj->transform.scale.x <= minScale)
			{
				isVisible = false;
			}
		}
	}
	else
	{
		parentObj->transform.scale = glm::vec3(1.0f);
	}

	parentObj->meshes[0].visible = isVisible;
	parentObj->meshes[0].shadowCaster = isVisible;

	parentObj->transform.RecalculateMatrix();
}

void Collectable::Collect()
{
	//assumes one mesh
	if (!isCollected)
	{
		isCollected = true;
	}
}