#include"../engine_headers/Material.h"

Material::Material(Shader& shader, std::vector<Texture>& textures)
{
	Material::shader = shader;
	Material::textures = textures;
}

void Material::SetTextures()
{
	shader.Activate();
	textures[0].SetUniform(shader, "tex0", 0);
	textures[0].Bind();

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