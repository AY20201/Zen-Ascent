#include"../engine_headers/Texture.h"

Texture::Texture(const char* imageFile, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = textureType;

	int imgWidth, imgHeight, numColChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(imageFile, &imgWidth, &imgHeight, &numColChannels, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	texUnit = slot;
	glBindTexture(type, ID);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(type, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(type);

	stbi_image_free(bytes);
	Unbind();
}

void Texture::SetUniform(Shader& shader, const char* uniformName, GLuint unit)
{
	GLuint texID = glGetUniformLocation(shader.ID, uniformName);
	shader.Activate();
	glUniform1i(texID, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}