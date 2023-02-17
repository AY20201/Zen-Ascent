#ifndef COLLECTABLE_CLASS_H
#define COLLECTABLE_CLASS_H

#include"../engine_headers/GameObject.h"
#include"../engine_headers/Behavior.h"

class Collectable : public Behavior
{
public:

	float rotateSpeed;
	float scaleDecreaseSpeed;

	float minScale;

	float hoverSpeed;
	float hoverScale;

	bool isCollected = false;
	bool isVisible = true;

	glm::vec3 originalPosition;

	Collectable(bool isCollected, float rotateSpeed = 70.0f, float scaleDecreaseSpeed = 2.5f, float minScale = 0.1f);
	void Awake() override;
	void Update(float deltaTime, GLFWwindow* window) override;
	void Collect();
};

#endif
