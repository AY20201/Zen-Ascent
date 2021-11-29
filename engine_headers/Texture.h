#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"Shader.h"

class Texture
{
	public:
		GLuint ID;
		GLenum type;
		GLuint texUnit;
		Texture() = default;
		Texture(const char* imageFile, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType);
		void SetUniform(Shader& shader, const char* uniformName, GLuint unit);
		void Bind();
		void Unbind();
		void Delete();
};

#endif