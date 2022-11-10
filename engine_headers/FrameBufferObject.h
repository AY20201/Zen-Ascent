#ifndef FRAMEBUFFEROBJECT_CLASS_H
#define FRAMEBUFFEROBJECT_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"Texture.h"
#include"Mesh.h"

const Vertex quadVertices[] =
{
	Vertex{glm::vec3(-1.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
	
	Vertex{glm::vec3(-1.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f)}
};

struct TextureObject
{
	GLuint textureID;
	GLuint textureUnit;
};

class FrameBufferObject
{
public:
	FrameBufferObject(int screenWidth, int screenHeight);
	
	int screenWidth, screenHeight;

	VAO fullscreenQuadVAO;
	std::vector<Vertex> quadVerts;

	GLuint bufferID;
	TextureObject depthTexture;
	TextureObject colorTexture;
	//GLuint textureID;
	//GLuint textureUnit;

	//GLuint CreateDepthTextureAttachment();
	//GLuint CreateDepthBufferAttachment();

	void AttachDepthTexture();
	void AttachColorTexture();
	void BindTexture(TextureObject& texObj);
	void UnbindTexture();
	void BindFrameBuffer();
	void UnbindFrameBuffer();
	void SetTexture(TextureObject& texObj, Shader& shader, const char* uniformName);
	
	void InitializeRenderQuad();
	void RenderQuad(Shader& shader);

	void Delete();

	//Texture* GetRefractionTexture();
	//Texture* GetRefractionDepthTexture();
};

#endif
