#include"Chunk.h"
#include"World.h"

Chunk::Chunk()
{
	id = -1;
}

Chunk::Chunk(glm::vec2 position, Material material)
{
	Chunk::position = position;
	Chunk::material = material;

	std::vector<std::vector<int>> yzDimension(VoxelData::chunkSizeY, std::vector<int>(VoxelData::chunkSizeZ, 0));
	voxelMap = std::vector<std::vector<std::vector<int>>>(VoxelData::chunkSizeX, yzDimension);
	Chunk::id = 0;

	PopulateVoxelMap();
	
}

void Chunk::PopulateVoxelMap()
{
	for (int x = 0; x < VoxelData::chunkSizeX; x++)
	{
		for (int y = 0; y < VoxelData::chunkSizeY; y++)
		{
			for (int z = 0; z < VoxelData::chunkSizeZ; z++)
			{
				voxelMap[x][y][z] = World::Instance.DetermineBlockByHeight(x + int(position.x), y, z + int(position.y));
			}
		}
	}
}

void Chunk::SetMesh()
{
	for (int x = 0; x < VoxelData::chunkSizeX; x++)
	{
		for (int y = 0; y < VoxelData::chunkSizeY; y++)
		{
			for (int z = 0; z < VoxelData::chunkSizeZ; z++)
			{
				if (World::blockTypes[voxelMap[x][y][z]].solid == 1)
				{
					CreateMesh(glm::vec3(x, y, z));
				}
			}
		}
	}

	Chunk::mesh = Mesh(vertices, indices, material);
	GameObject::Instantiate(glm::vec3(position.x / VoxelData::scaleFactor, 0.0f, position.y / VoxelData::scaleFactor), glm::vec3(0.0f), glm::vec3(1.0f), mesh);
}

bool Chunk::CheckVoxel(glm::vec3 pos)
{	
	//Block exteriorBlock = World::Instance.GetVoxelFromWorldVec3(pos + glm::vec3(Chunk::position.x, 0.0f, Chunk::position.y));

	//return VoxelInChunk(pos);

	
	if (!VoxelInChunk(pos))
	{
		int exteriorBlock = World::Instance.GetVoxelFromWorldVec3(pos + glm::vec3(Chunk::position.x, 0, Chunk::position.y));
		return World::blockTypes[exteriorBlock].solid;
	}

	return World::blockTypes[voxelMap[int(pos.x)][int(pos.y)][int(pos.z)]].solid;
}

int Chunk::GetVoxelFromVec3(glm::vec3 pos)
{
	float x = pos.x;
	float z = pos.z;

	x -= position.x;
	z -= position.y;

	if (!VoxelInChunk(glm::vec3(x, pos.y, z)))
	{
		return 0;
	}

	return voxelMap[int(x)][int(pos.y)][int(z)];
}

bool Chunk::VoxelInChunk(glm::vec3 pos)
{
	bool inX = pos.x > VoxelData::chunkSizeX - 1 || pos.x < 0.0f;
	bool inY = pos.y > VoxelData::chunkSizeY - 1 || pos.y < 0.0f;
	bool inZ = pos.z > VoxelData::chunkSizeZ - 1 || pos.z < 0.0f;
	return !(inX || inY || inZ);
}


void Chunk::CreateMesh(glm::vec3 pos)
{
	for (int face = 0; face < 6; face++)
	{
		if (!CheckVoxel(pos + VoxelData::faceChecks[face]))
		{
			int block = voxelMap[int(pos.x)][int(pos.y)][int(pos.z)];

			AddTexture(World::blockTypes[block].GetTextureId(face));
			AddVertex(pos / VoxelData::scaleFactor, face * 4, int(uvs.size()) - 4);
			AddVertex(pos / VoxelData::scaleFactor, face * 4 + 1, int(uvs.size()) - 3);
			AddVertex(pos / VoxelData::scaleFactor, face * 4 + 2, int(uvs.size()) - 2);
			AddVertex(pos / VoxelData::scaleFactor, face * 4 + 3, int(uvs.size()) - 1);

			indices.push_back(vertexIndex + 2);
			indices.push_back(vertexIndex + 1);
			indices.push_back(vertexIndex);
			indices.push_back(vertexIndex + 3);
			indices.push_back(vertexIndex + 2);
			indices.push_back(vertexIndex);
			
			vertexIndex += 4;
		}
	}
}

void Chunk::AddVertex(glm::vec3 pos, int index, int uvIndex)
{
	Vertex vertex = VoxelData::voxelVertices[index];
	Chunk::vertices.push_back(Vertex{ pos + vertex.position, uvs[uvIndex], vertex.normal });
}

void Chunk::AddTexture(int texId)
{
	int x = texId % VoxelData::atlasSize;
	int y = (texId - (texId % VoxelData::atlasSize)) / VoxelData::atlasSize;

	float xUv = float(x) * VoxelData::normalizedTextureSize;
	float yUv = 1.0f - (float(y) * VoxelData::normalizedTextureSize);

	
	yUv -= VoxelData::normalizedTextureSize;

	uvs.push_back(glm::vec2(xUv + VoxelData::normalizedTextureSize, yUv));
	uvs.push_back(glm::vec2(xUv, yUv));
	uvs.push_back(glm::vec2(xUv, yUv + VoxelData::normalizedTextureSize));
	uvs.push_back(glm::vec2(xUv + VoxelData::normalizedTextureSize, yUv + VoxelData::normalizedTextureSize));
	
}