//#include "shaderc.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>

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
	std::ifstream i("shaders.json");
	json j;
	i >> j;

	std::cout << " Vertex shaders: ------------------------------------------\n";
	for (auto& element : j["vertex"])
		print(element, "vert");

	std::cout << "\n Fragment shaders: ----------------------------------------\n";
	for (auto& element : j["fragment"])
		print(element, "frag");

	std::cout << "\n Compute shaders: -----------------------------------------\n";
	for (auto& element : j["compute"])
		print(element, "comp");
}
