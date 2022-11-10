#include"../engine_headers/FrameBufferObject.h"

FrameBufferObject::FrameBufferObject(int screenWidth, int screenHeight) {
	FrameBufferObject::screenWidth = screenWidth;
	FrameBufferObject::screenHeight = screenHeight;
	
	//textureUnit = Texture::activeTexUnit;
	//Texture::activeTexUnit++;

	glGenFramebuffers(1, &bufferID);

	AttachDepthTexture();
	AttachColorTexture();
	/*
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	*/

	UnbindFrameBuffer();
	UnbindTexture();
}

void FrameBufferObject::AttachDepthTexture()
{
	depthTexture.textureUnit = Texture::activeTexUnit;
	Texture::activeTexUnit++;

	glGenTextures(1, &depthTexture.textureID);
	glBindTexture(GL_TEXTURE_2D, depthTexture.textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.textureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void FrameBufferObject::AttachColorTexture()
{
	colorTexture.textureUnit = Texture::activeTexUnit;
	Texture::activeTexUnit++;

	glGenTextures(1, &colorTexture.textureID);
	glBindTexture(GL_TEXTURE_2D, colorTexture.textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.textureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void FrameBufferObject::Delete() {
	glDeleteFramebuffers(1, &bufferID);
	glDeleteTextures(1, &depthTexture.textureID);
	glDeleteTextures(1, &colorTexture.textureID);
}

void FrameBufferObject::BindTexture(TextureObject& texObj)
{
	glActiveTexture(GL_TEXTURE0 + texObj.textureUnit);
	glBindTexture(GL_TEXTURE_2D, texObj.textureID);
}

void FrameBufferObject::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferObject::UnbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
}

void FrameBufferObject::BindFrameBuffer() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glViewport(0, 0, screenWidth, screenHeight);
}

void FrameBufferObject::SetTexture(TextureObject& texObj, Shader& shader, const char* uniformName)
{
	shader.Activate();
	glUniform1i(glGetUniformLocation(shader.ID, uniformName), texObj.textureUnit);
	BindTexture(texObj);
}

void FrameBufferObject::InitializeRenderQuad()
{
	quadVerts = std::vector<Vertex>(quadVertices, quadVertices + sizeof(quadVertices) / sizeof(Vertex));

	fullscreenQuadVAO.Bind();
	VBO vbo(quadVerts);

	fullscreenQuadVAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	fullscreenQuadVAO.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	fullscreenQuadVAO.Unbind();
	vbo.Unbind();
}

void FrameBufferObject::RenderQuad(Shader& shader)
{
	shader.Activate();

	fullscreenQuadVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

/*
GLuint FrameBufferObject::CreateDepthTextureAttachment() {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

	return texture;
}
*/

/*
GLuint FrameBufferObject::CreateDepthBufferAttachment() {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	return depthBuffer;
}
*/