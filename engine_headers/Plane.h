#ifndef PLANEGENERATOR_CLASS_H
#define PLANEGENERATOR_CLASS_H

#include"GameObject.h"

class Plane
{
public:
	float size = 1.0f;
	int resolution = 1;
	float textureScale = 1.0f;

	Mesh mesh;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> uvs;

	Plane(float size, int resolution, float textureScale, Material* mat);
	Plane() = default;
	//bool MeshInitialized();
};

#endif