#include <fstream>

#include "Shader.h"

ColoredConsole console;

int main()
{
	std::ifstream i("shaders.json");
	nlohmann::json j;
	i >> j;

	std::vector<Shader> shaders;

	for (auto& element : j["vertex"])
		shaders.emplace_back(element, "vert");
	for (auto& element : j["fragment"])
		shaders.emplace_back(element, "frag");
	for (auto& element : j["compute"])
		shaders.emplace_back(element, "comp");

	for (auto& shader : shaders)
	{
		shader.compile(console);
	}
}
