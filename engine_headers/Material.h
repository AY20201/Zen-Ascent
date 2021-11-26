#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include"Texture.h"
#include"Shader.h"

#include<vector>

class Material
{
	public:

		Shader shader;
		std::vector<Texture> textures;

		Material(Shader& shader, std::vector<Texture>& textures);
		Material() = default;
		void SetTextures();

};

#endif
