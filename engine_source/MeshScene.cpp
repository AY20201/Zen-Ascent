#include"../engine_headers/MeshScene.h"

MeshScene::MeshScene(Transform transform, Behavior* behavior, std::vector<const char*> lodFilenames, Shader& shader, Material* material)
{
	MeshScene::transform = transform;
	MeshScene::shader = shader;
	MeshScene::material = material;

	for (unsigned int i = 0; i < lodFilenames.size(); i++)
	{
		lodMeshes.push_back(std::vector<Mesh>{});
		ImportMeshes(lodFilenames[i], i);
	}

	/*
	for (unsigned int i = 0; i < lodMeshes[lodMeshes.size() - 1].size(); i++)
	{
		Mesh sceneMesh = lodMeshes[lodMeshes.size() - 1][i];
		CollisionMesh newColMesh(sceneMesh.vertices, sceneMesh.indices, transform.matrix, sceneGameObject);
		sceneCollisionMeshes.push_back(newColMesh);
	}
	*/

	GameObject* instantiatedGO = new GameObject(transform.position, transform.rotation, transform.scale, lodMeshes[0], behavior);
	sceneGameObject = instantiatedGO;
}

void MeshScene::ImportMeshes(const char* filename, unsigned int activeLod)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (scene)
	{
		if (material != nullptr)
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				InitializeMesh(activeLod, scene->mMeshes[i], material);
			}
		}
		else
		{
			if (sceneMaterials.size() == 0)
			{
				InitializeMaterials(scene, std::string(filename));
			}

			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				InitializeMesh(activeLod, scene->mMeshes[i], sceneMaterials[scene->mMeshes[i]->mMaterialIndex]);
			}
		}
	}
	else
	{
		std::cout << importer.GetErrorString() << std::endl;
	}
}
/*
std::vector<CollisionMesh> MeshScene::ImportCollisionMesh(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	std::vector<CollisionMesh> importedCMeshes;

	if (scene)
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			CollisionMesh importedCMesh = InitializeCollisionMesh(scene->mMeshes[i]);
			importedCMeshes.push_back(importedCMesh);
		}
	}

	return importedCMeshes;
}
*/
/*
void MeshScene::ImportCollisionMeshes()
{
	for (int i = 0; i < sceneMeshes.size(); i++)
	{
		CollisionMesh newColMesh(sceneMeshes[i].vertices, sceneMeshes[i].indices, sceneGameObject);
		sceneCollisionMeshes.push_back(newColMesh);
	}
}
*/

void MeshScene::InitializeMesh(unsigned int activeLod, const aiMesh* mesh, Material* material)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	const aiVector3D Vec0(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D pos = mesh->mVertices[i];
		const aiVector3D texcoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : Vec0;
		const aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[i] : Vec0;
		const aiVector3D tangent = mesh->HasTangentsAndBitangents() ? mesh->mTangents[i] : Vec0;

		Vertex newVert = Vertex{ 
			glm::vec3(pos.x, pos.y,  pos.z), 
			glm::vec2(texcoord.x, texcoord.y), 
			glm::vec3(normal.x, normal.y, normal.z),
			glm::vec3(tangent.x, tangent.y, tangent.z) 
		};

		vertices.push_back(newVert);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	Mesh newMesh(vertices, indices, material, true);
	lodMeshes[activeLod].push_back(newMesh);
}
/*
void MeshScene::InitializeCollisionMesh(const aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	const aiVector3D Vec0(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D pos = mesh->mVertices[i];

		Vertex newVert = Vertex{
			glm::vec3(pos.x, pos.y,  pos.z),
		};

		vertices.push_back(newVert);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	CollisionMesh newMesh(vertices, indices, nullptr);
	sceneCollisionMeshes.push_back(newMesh);
}
*/

void MeshScene::InitializeMaterials(const aiScene* scene, const std::string& filename)
{
	std::string directory = filename.substr(0, filename.find_last_of("/"));

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* aiMat = scene->mMaterials[i];

		Texture* albedo = InitializeTexture(aiMat, aiTextureType_DIFFUSE, directory);
		Texture* normalMap = InitializeTexture(aiMat, aiTextureType_HEIGHT, directory);
		//Texture* specularMap = InitializeTexture(aiMat, aiTextureType_SPECULAR, directory);

		aiColor3D diffuseColor(1.0, 1.0, 1.0);
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

		Material* newMaterial = new Material(shader, TexParam{ "albedo", albedo, 1.0, glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b) }, TexParam{ "normalMap", normalMap, 1.0 }/*, TexParam{"specMap", specularMap, 1.0}*/);
		sceneMaterials.push_back(newMaterial);
	}
}

Texture* MeshScene::InitializeTexture(const aiMaterial* mat, aiTextureType type, std::string dir)
{
	if (mat->GetTextureCount(type) > 0)
	{
		aiString path;

		if (mat->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string fullPath = dir + std::string("/") + std::string(path.data);
			Texture* matTex = new Texture(fullPath.c_str(), GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_UNSIGNED_BYTE);
			return matTex;
		}
	}
	else
	{
		switch (type)
		{
			case aiTextureType_DIFFUSE:
				return Texture::defaultAlbedo;
			case aiTextureType_HEIGHT:
				return Texture::defaultNormalMap;
			case aiTextureType_NORMALS:
				return Texture::defaultNormalMap;
			//case aiTextureType_SPECULAR:
				//return Texture::defaultSpecMap;
		}
	}

	return new Texture();
}

void MeshScene::Clear()
{
	delete sceneGameObject;

	for (unsigned int i = 0; i < sceneMaterials.size(); i++)
	{
		//sceneMaterials[i]->Clear();
		delete sceneMaterials[i];
	}
}