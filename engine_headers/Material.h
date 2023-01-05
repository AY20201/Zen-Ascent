#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include<glm/glm.hpp>
#include"Texture.h"
#include"Shader.h"

#include<vector>

struct TexParam
{
	const char* uniformName;
	Texture* tex;
	float scale;
	glm::vec3 color = glm::vec3(1, 1, 1);

	bool Empty()
	{
		return uniformName == "" || tex == nullptr;
	}
};

class Material
{
	public:

		Shader shader;
		
		TexParam albedo;
		TexParam normalMap;
		//TexParam specMap;
		bool transparent;

		Material(Shader& shader, TexParam albedo, TexParam normalMap, bool transparent/*, TexParam specMap*/);
		Material() = default;
		void SetTextures();
		void Clear();

	private:

		void SetTexture(Texture* tex, const char* uniName);
};

#endif
