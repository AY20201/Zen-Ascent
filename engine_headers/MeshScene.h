#ifndef MESHIMPORTER_CLASS_H
#define MESHIMPORTER_CLASS_H

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"GameObject.h"

class MeshScene
{
	public:

		std::vector<Mesh> sceneMeshes;
		std::vector<Material*> sceneMaterials;

		Transform transform;
		const char* filename;
		Shader shader;
		Material* material;

		MeshScene(Transform transform, const char* filename, Shader& shader, Material* material);
		void ImportMeshes();
		void Clear();
		//void InstantiateGameObjects();

	private:
		std::vector<GameObject*> sceneGameObjects;

		void InitializeMesh(unsigned int index, const aiMesh* mesh, Material* material);
		void InitializeMaterials(const aiScene* scene, const std::string& filename);
		Texture* InitializeTexture(const aiMaterial* mat, aiTextureType type, std::string dir);

};

#endif

