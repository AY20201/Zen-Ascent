#include"../engine_headers/SSAO.h"

SSAO::SSAO(int ssaoKernelSize)
{
	SSAO::ssaoKernelSize = ssaoKernelSize;
}

void SSAO::InitializeKernels()
{
	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;

	for (int i = 0; i < 16; i++)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 16.0f;
		scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
		sample *= scale;

		ssaoKernel.push_back(sample);
	}

	for (int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			0.0f);
		ssaoNoise.push_back(glm::normalize(noise));
	}

	noiseTextureUnit = Texture::activeTexUnit;
	Texture::activeTexUnit++;

	glGenTextures(1, &noiseTextureID);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAO::SetTexture(GLuint texID, GLuint texUnit, Shader& shader, const char* uniformName)
{
	shader.Activate();
	glUniform1i(glGetUniformLocation(shader.ID, uniformName), texUnit);
	
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texID);
}

void SSAO::SetSSAOKernel(Shader& shader)
{
	shader.Activate();

	for (unsigned int i = 0; i < ssaoKernel.size(); i++)
	{
		glUniform3f(glGetUniformLocation(shader.ID, ("samples[" + std::to_string(i) + "]").c_str()), ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
	}
}