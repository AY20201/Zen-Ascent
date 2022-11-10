#include"../engine_headers/Plane.h"

Plane::Plane(float size, int resolution, float textureScale, Material* mat)
{
	Plane::size = size;
	Plane::resolution = resolution;
	Plane::textureScale = textureScale;

	for (int z = 0; z <= resolution; z++)
	{
		for (int x = 0; x <= resolution; x++)
		{
			vertices.push_back(glm::vec3(x * (size / (float)resolution), 0.0f, z * (size / (float)resolution)));
		}
	}

	int vert = 0;
	for (int z = 0; z < resolution; z++)
	{
		for (int x = 0; x < resolution; x++)
		{
			indices.push_back(vert + resolution + 1);
			indices.push_back(vert + 1);
			indices.push_back(vert + 0);
			indices.push_back(vert + resolution + 1);
			indices.push_back(vert + resolution + 2);
			indices.push_back(vert + 1);

			vert++;
		}
		vert++;
	}
	/*
	for (int z = 0; z <= resolution; z++)
	{
		for (int x = 0; x <= resolution; x++)
		{
			uvs.push_back(glm::vec2((float)x / (float)resolution, (float)z / (float)resolution));
		}
	}
	*/
	for (int i = 0; i < (resolution + 1) * (resolution + 1); i++)
	{
		uvs.push_back(glm::vec2(vertices[i].x * textureScale, vertices[i].z * textureScale));
	}

	std::vector<Vertex> verticesFull;
	for (int i = 0; i < (resolution + 1) * (resolution + 1); i++)
	{
		verticesFull.push_back(Vertex{ vertices[i], uvs[i], glm::vec3(0.0f, 1.0f, 0.0f) });
	}

	mesh = Mesh(verticesFull, indices, mat, false);
}

//correct file location