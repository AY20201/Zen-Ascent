#include"../engine_headers/GameSaver.h"

void GameSaver::Save(std::ofstream& file, int data)
{
	file.write(reinterpret_cast<char*>(&data), sizeof(int));
}

void GameSaver::Save(std::ofstream& file, float data)
{
	file.write(reinterpret_cast<char*>(&data), sizeof(float));
}

void GameSaver::Save(std::ofstream& file, bool data)
{
	file.write(reinterpret_cast<char*>(&data), sizeof(bool));
}

void GameSaver::Save(std::ofstream& file, glm::vec3 data)
{
	file.write(reinterpret_cast<char*>(&(data.x)), sizeof(float));
	file.write(reinterpret_cast<char*>(&(data.y)), sizeof(float));
	file.write(reinterpret_cast<char*>(&(data.z)), sizeof(float));
}

void GameSaver::Load(std::ifstream& file, int* data)
{
	file.read(reinterpret_cast<char*>(data), sizeof(int));
}

void GameSaver::Load(std::ifstream& file, float* data)
{
	file.read(reinterpret_cast<char*>(data), sizeof(float));
}

void GameSaver::Load(std::ifstream& file, bool* data)
{
	file.read(reinterpret_cast<char*>(data), sizeof(bool));
}

void GameSaver::Load(std::ifstream& file, glm::vec3* data)
{
	file.read(reinterpret_cast<char*>(&(data->x)), sizeof(float));
	file.read(reinterpret_cast<char*>(&(data->y)), sizeof(float));
	file.read(reinterpret_cast<char*>(&(data->z)), sizeof(float));
}