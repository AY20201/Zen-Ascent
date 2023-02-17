#ifndef TRANSITIONER_CLASS_H
#define TRANSITIONER_CLASS_H

#include"Texture.h"
#include"Shader.h"
#include<vector>

class Transitioner
{
public:
	/*
	enum Transition
	{
		TRANSITION_UP_DOWN,
		TRANSITION_SWIPE_UP_RIGHT,
		TRANSITION_SWIPE_DOWN_RIGHT,
		TRANSITION_SWIPE_RIGHT,
		TRANSITON_RANDOM
	};
	*/

	static Transitioner Instance;

	float threshold = 0.0f;
	float transitionSpeed = 1.0f;
	int currentTransition = 0;
	Shader transitionShader;

	bool inTransitionStart;
	bool inTransitionEnd;

	std::vector<Texture*> transitionTextures;
	void InitializeTransitions(std::vector<const char*> filenames, Shader& transitionShader, float transitionSpeed);
	void DeleteTransitions();

	void SetTransition(int transition, bool random);
	void Update(float deltaTime);
};

#endif 
