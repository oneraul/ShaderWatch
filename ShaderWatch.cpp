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
		if (!std::filesystem::exists("compiled"))
		{
			std::filesystem::create_directory("compiled");
			console.print("Created directory compiled.\n", rmkl::ColoredConsole::Color::Grey);
		}

		std::string jsonFile = "shaders.json";
		if (!std::filesystem::exists(jsonFile))
		{
			console.print("shaders.json not found.", rmkl::ColoredConsole::Color::Red);
			std::cin >> jsonFile;
			return -1;
		}

		std::ifstream i(jsonFile);
		nlohmann::json json;
		i >> json;

		for (auto& element : json["vertex"])
			shaders.emplace_back(element, rmkl::Shader::Type::Vertex);
		for (auto& element : json["fragment"])
			shaders.emplace_back(element, rmkl::Shader::Type::Fragment);
		for (auto& element : json["compute"])
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
