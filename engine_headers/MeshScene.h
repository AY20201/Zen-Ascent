#ifndef MESHIMPORTER_CLASS_H
#define MESHIMPORTER_CLASS_H

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"GameObject.h"
#include"CollisionMesh.h"

class MeshScene
{
	public:

		std::vector<std::vector<Mesh>> lodMeshes;// sceneMeshes;
		std::vector<Material*> sceneMaterials;

		std::vector<CollisionMesh> sceneCollisionMeshes;

		std::vector<GameObject*> sceneGameObjects;

		Transform transform;
		Shader shader;
		Material* material;

		MeshScene(Transform transform, Behavior* behavior, std::vector<const char*> lodFilenames, Shader& shader, Material* material, bool separatedGOs, bool hasBoxColliders);
		void ImportMeshes(const char* filename, unsigned int activeLod);
		void Clear();

		//static std::vector<CollisionMesh> ImportCollisionMesh(const char* filename);
		//void ImportCollisionMeshes();

	private:

		void InitializeMesh(unsigned int activeLod, const aiMesh* mesh, Material* material);
		void InitializeMaterials(const aiScene* scene, const std::string& filename);
		Texture* InitializeTexture(const aiMaterial* mat, aiTextureType type, std::string dir);

		//void InitializeCollisionMesh(const aiMesh* mesh);

};

#endif

