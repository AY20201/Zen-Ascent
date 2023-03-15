#ifndef TEXTRENDERER_CLASS_H
#define TEXTRENDERER_CLASS_H

#include<iostream>
#include<glad/glad.h>

#include<ft2build.h>
#include FT_FREETYPE_H

#include"Transform.h"
#include"Texture.h"
#include"Shader.h"

#include<map>

struct Character
{
	GLuint texId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class TextRenderer
{
	public:
		FT_Library ft;
		FT_Face fontFace;
		std::map<char, Character> characters;

		GLuint textVAO = 0;
		GLuint textVBO = 0;

		GLuint textureSlot;

		glm::mat4 projection;
		float screenWidth;
		float screenHeight;

		TextRenderer(const char* fontFilename, int fontSize, float screenWidth, float screenHeight);
		void LoadCharacters();
		void LoadVertexData();
		void RenderText(Shader& textShader, std::string text, float x, float y, float scale, bool centerX, bool center3d, glm::mat4 view, glm::mat4 proj, Transform transform, glm::vec3 color);
};

#endif
