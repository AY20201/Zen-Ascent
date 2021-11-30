#include"../engine_headers/Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::material = material;

	vao.Bind();

	VBO vbo(Mesh::vertices);
	EBO ebo(Mesh::indices);

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

}

void Mesh::Draw(Shader& shader, glm::mat4 matrix)
{
	shader.Activate();
	vao.Bind();

	material.SetTextures();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
	
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}
