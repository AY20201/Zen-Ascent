#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include"Texture.h"
#include"Shader.h"

#include<vector>

class Material
{
	public:

		Shader shader;
		Texture texture;

		Material(Shader& shader, Texture& textures);
		Material() = default;
		void SetTextures();

};

#endif
