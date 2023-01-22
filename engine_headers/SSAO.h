#ifndef SSAO_CLASS_H
#define SSAO_CLASS_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Shader.h"
#include"Texture.h"
#include"Transform.h"
#include<random>

class SSAO
{
public:
	std::vector<glm::vec3> ssaoKernel;
	std::vector<glm::vec3> ssaoNoise;

	int ssaoKernelSize = 32;

	GLuint noiseTextureID;
	GLuint noiseTextureUnit;

	SSAO(int ssaoKernelSize);
	void InitializeKernels();

	void SetTexture(GLuint texID, GLuint texUnit, Shader& shader, const char* uniformName);
	void SetSSAOKernel(Shader& shader);
};

#endif
