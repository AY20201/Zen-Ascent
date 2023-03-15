#include"../engine_headers/TextRenderer.h"

TextRenderer::TextRenderer(const char* fontFilename, int fontSize, float screenWidth, float screenHeight)
{
	FT_Init_FreeType(&ft);
	FT_New_Face(ft, fontFilename, 0, &fontFace);
	FT_Set_Pixel_Sizes(fontFace, 0, (int)(fontSize * (screenHeight / 2160.0f)));

	textureSlot = Texture::activeTexUnit;
	Texture::activeTexUnit++;

	TextRenderer::screenWidth = screenWidth;
	TextRenderer::screenHeight = screenHeight;

	projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
}

void TextRenderer::LoadCharacters()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
		{
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		Character character = {
			texture,
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			static_cast<unsigned int>(fontFace->glyph->advance.x)
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(fontFace);
	FT_Done_FreeType(ft);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void TextRenderer::LoadVertexData()
{
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);

	glBindVertexArray(textVAO);

	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void TextRenderer::RenderText(Shader& textShader, std::string text, float x, float y, float scale, bool centerX, bool center3d, glm::mat4 view, glm::mat4 proj, Transform transform, glm::vec3 color)
{
	textShader.Activate();
	glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(transform.matrix));
	
	glUniform1i(glGetUniformLocation(textShader.ID, "text"), textureSlot);
	glActiveTexture(GL_TEXTURE0 + textureSlot);

	glBindVertexArray(textVAO);

	glm::vec2 textOffset = glm::ivec2(0.0f, 0.0f);
	if (centerX || center3d)
	{
		float textWidth = 0.0f;
		std::string::const_iterator i;
		for (i = text.begin(); i != text.end(); i++)
		{
			Character ch = characters[*i];
			textWidth += (ch.advance >> 6) * scale;
		}

		if (centerX)
		{
			textOffset.x = (screenWidth - textWidth) / 2.0f;
		}

		if (center3d)
		{
			textOffset.x = -textWidth / 2.0f;
		}
	}

	x += textOffset.x;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texId);

		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}