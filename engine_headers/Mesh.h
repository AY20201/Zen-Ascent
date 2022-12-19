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
		Material* material = new Material();

		VAO vao;

		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material* material, bool tangentsCalculated);
		void Draw(glm::mat4 matrix);
		void Draw(glm::mat4 matrix, Shader& shader);
		Mesh() = default;

	private:
		void CalculateTangents();
};

#endif

