#include"../engine_headers/Transitioner.h"

Transitioner Transitioner::Instance;

void Transitioner::InitializeTransitions(std::vector<const char*> filenames, Shader& transitionShader, float transitionSpeed)
{
	for(unsigned int i = 0; i < filenames.size(); i++)
	{
		Texture* newTex = new Texture(filenames[i], GL_TEXTURE_2D, GL_NEAREST, /*1,*/ GL_RGBA, GL_UNSIGNED_BYTE); //All GL_RGBA
		transitionTextures.push_back(newTex);
	}

	Transitioner::transitionShader = transitionShader;
	Transitioner::transitionSpeed = transitionSpeed;
}

void Transitioner::DeleteTransitions()
{
	for (unsigned int i = 0; i < transitionTextures.size(); i++)
	{
		delete transitionTextures[i];
	}
}

void Transitioner::SetTransition(int transition, bool random)
{
	currentTransition = random ? rand() % (int)transitionTextures.size() : transition;
	
	Texture* currentTex = transitionTextures[currentTransition];

	//setting the texture
	transitionShader.Activate();
	glUniform1i(glGetUniformLocation(transitionShader.ID, "transScreen"), currentTex->texUnit);

	glActiveTexture(GL_TEXTURE0 + currentTex->texUnit);
	glBindTexture(GL_TEXTURE_2D, currentTex->ID);
}

void Transitioner::Update(float deltaTime)
{
	SetTransition(currentTransition, false);

	if (inTransitionEnd)
	{
		threshold -= transitionSpeed * deltaTime;

		if (threshold <= 0.0f)
		{
			threshold = 0.0f;
			inTransitionEnd = false;
			inTransitionStart = false;
		}
	}
	else if (inTransitionStart)
	{
		threshold += transitionSpeed * deltaTime;

		if (threshold >= 2.0f)
		{
			threshold = 2.0f;
			inTransitionEnd = true;
			inTransitionStart = false;
		}
	}

	threshold = GLclampf(threshold);

	transitionShader.Activate();
	glUniform1f(glGetUniformLocation(transitionShader.ID, "threshold"), threshold);
}