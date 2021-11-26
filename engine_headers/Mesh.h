#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include"Transform.h"
#include"Shader.h"
#include"Material.h"

#include<vector>

class Mesh
{
	public:
		std::vector <Vertex> vertices;
		std::vector <GLuint> indices;
		Material material;

		VAO vao;

		Mesh(Vertex vertices[], GLuint indices[], Material& material);
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material);
		void Draw(Shader& shader, glm::mat4 matrix);
		Mesh() = default;
};

#endif

