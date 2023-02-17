#ifndef GAMESAVER_CLASS_H
#define GAMESAVER_CLASS_H

#include<iostream>
#include<fstream>
#include"Transform.h"

class GameSaver
{
public:

	static void Save(std::ofstream& file, int data);
	static void Save(std::ofstream& file, float data);
	static void Save(std::ofstream& file, bool data);
	static void Save(std::ofstream& file, glm::vec3 data);

	static void Load(std::ifstream& file, int* data);
	static void Load(std::ifstream& file, float* data);
	static void Load(std::ifstream& file, bool* data);
	static void Load(std::ifstream& file, glm::vec3* data);
	
};

#endif
