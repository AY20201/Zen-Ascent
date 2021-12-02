#include"../engine_headers/Material.h"

Material::Material(Shader& shader, Texture& texture)
{
	Material::shader = shader;
	Material::texture = texture;
}

void Material::SetTextures()
{
	shader.Activate();
	texture.SetUniform(shader, "tex", 0);
	texture.Bind();

	/*
	unsigned int numTextures = 0;

	for (int i = 0; i < textures.size(); i++)
	{
		std::string num;
		num = std::to_string(numTextures++);
		textures[i].SetUniform(shader, ("tex" + num).c_str(), i);
	}
	*/
}