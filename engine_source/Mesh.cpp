#include"../engine_headers/Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material* material, bool tangentsCalculated, bool uvsCalculated, bool shadowCaster)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::material = material;
	Mesh::shadowCaster = shadowCaster;

	if (!tangentsCalculated)
	{
		CalculateTangents();
	}

	if (!uvsCalculated)
	{
		CalculateUVS();
	}
	
	vao.Bind();

	VBO vbo(Mesh::vertices);
	EBO ebo(Mesh::indices);

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
	vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

}

// code from https://ogldev.org/www/tutorial26/tutorial26.html
void Mesh::CalculateTangents()
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];

		glm::vec3 edge1 = v1.position - v0.position;
		glm::vec3 edge2 = v2.position - v0.position;

		float deltaU1 = v1.texcoord.x - v0.texcoord.x;
		float deltaV1 = v1.texcoord.y - v0.texcoord.y;
		float deltaU2 = v2.texcoord.x - v0.texcoord.x;
		float deltaV2 = v2.texcoord.y - v0.texcoord.y;

		float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		glm::vec3 tangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;
	}
}

void Mesh::CalculateUVS()
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		Vertex& vert = vertices[i];
		glm::vec3 norm = glm::normalize(vert.normal);

		if (norm == glm::vec3(0.0f, 1.0f, 0.0f) || norm == glm::vec3(0.0f, -1.0f, 0.0f)) //if up/down
		{
			glm::vec2 newUV = glm::vec2(vert.position.x, vert.position.z);
			vertices[i].texcoord = newUV;
		}
		else if (norm == glm::vec3(1.0f, 0.0f, 0.0f) //if right
			|| norm == glm::vec3(-1.0f, 0.0f, 0.0f))
		{
			glm::vec2 newUV = glm::vec2(vert.position.y, vert.position.z);
			vertices[i].texcoord = newUV;
		}
		
		else if (norm == glm::vec3(0.0f, 0.0f, 1.0f)
			|| norm == glm::vec3(0.0f, 0.0f, -1.0f))
		{
			glm::vec2 newUV = glm::vec2(vert.position.x, vert.position.y);
			vertices[i].texcoord = newUV;
		}
			

		
	}
}

void Mesh::Draw(glm::mat4 matrix)
{
	if (vertices.size() > 0)
	{
		material->shader.Activate();
		vao.Bind();

		material->SetTextures();
		glUniformMatrix4fv(glGetUniformLocation(material->shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
}

void Mesh::Draw(glm::mat4 matrix, Shader& shader)
{
	if (vertices.size() > 0)
	{
		shader.Activate();
		vao.Bind();

		//material->SetTextures();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
}
