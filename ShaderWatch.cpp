#include "json.hpp"
#include <iostream>
#include <fstream>
#include <windows.h>

#define intense FOREGROUND_INTENSITY |
#define background * 16
#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define purple 5
#define yellow 6
#define white 7
#define grey intense black
#define underscore | COMMON_LVB_UNDERSCORE
#define top_line | COMMON_LVB_GRID_HORIZONTAL
#define left_line | COMMON_LVB_GRID_LVERTICAL

using json = nlohmann::json;

void print(json& element, std::string suffix)
{
	if (element.is_string())
	{
		std::string name = element;
		std::cout << name << "." << suffix << " -o " << name << "_" << suffix << ".spv" << "\n";
	}
	else
	{
		std::string file = element["file"];
		for (auto& variant : element["variants"])
		{
			std::cout << file << "." << suffix;
			for (auto& define : variant["defines"])
				std::cout << " -D" << static_cast<std::string>(define);
			std::cout << " -o " << static_cast<std::string>(variant["outputFile"]) << "\n";
		}
	}
}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::ifstream i("shaders.json");
	json j;
	i >> j;

	SetConsoleTextAttribute(hConsole, white);
	std::cout << " Vertex shaders: ------------------------------------------\n";
	
	SetConsoleTextAttribute(hConsole, grey);
	for (auto& element : j["vertex"])
		print(element, "vert");

	SetConsoleTextAttribute(hConsole, white);
	std::cout << "\n Fragment shaders: ----------------------------------------\n";

	SetConsoleTextAttribute(hConsole, grey);
	for (auto& element : j["fragment"])
		print(element, "frag");

	SetConsoleTextAttribute(hConsole, white);
	std::cout << "\n Compute shaders: -----------------------------------------\n";

	SetConsoleTextAttribute(hConsole, grey);
	for (auto& element : j["compute"])
		print(element, "comp");


	SetConsoleTextAttribute(hConsole, white);
}
