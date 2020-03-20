#include <fstream>
#include <thread>
#include "Shader.h"

int main()
{
	rmkl::ColoredConsole console;
	shaderc::Compiler compiler;
	std::vector<rmkl::Shader> shaders;

	console.print("Initializing ShaderWatch.\n\n", rmkl::ColoredConsole::Color::White);
	{
		std::ifstream i("shaders.json");
		nlohmann::json j;
		i >> j;

		for (auto& element : j["vertex"])
			shaders.emplace_back(element, rmkl::Shader::Type::Vertex);
		for (auto& element : j["fragment"])
			shaders.emplace_back(element, rmkl::Shader::Type::Fragment);
		for (auto& element : j["compute"])
			shaders.emplace_back(element, rmkl::Shader::Type::Compute);

		for (auto& shader : shaders)
		{
			shader.compile(compiler, console, true);
		}
	}

	console.print("\nWatch started.\n\n", rmkl::ColoredConsole::Color::White);
	while (true)
	{
		for (auto& shader : shaders)
		{
			shader.compile(compiler, console);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
