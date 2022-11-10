#ifndef PLANEGENERATOR_CLASS_H
#define PLANEGENERATOR_CLASS_H

#include"GameObject.h"

class Plane
{
public:
	float size;
	int resolution;
	float textureScale;

	Mesh mesh;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> uvs;

	Plane(float size, int resolution, float textureScale, Material* mat);
	//bool MeshInitialized();
};

#endif

//correct file location