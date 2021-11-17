#include <fstream>
#include <thread>
#include "Shader.h"

int main()
{
	rmkl::ColoredConsole console;
	shaderc::Compiler compiler;
	std::vector<rmkl::Shader> shaders;

	console.print("Initializing ShaderWatch.\n", rmkl::ColoredConsole::Color::White);
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

		if (json.contains("version"))
		{
			const auto& version = json["version"];
			if (!version.is_number_float())
			{
				console.print("Invalid version specified", rmkl::ColoredConsole::Color::Red);
				return -1;
			}

			const float v = version;
			if (v == 1.0f)
			{
				console.print("Compiling shaders for Vulkan 1.0\n\n", rmkl::ColoredConsole::Color::White);
				rmkl::Shader::s_vulkanVersion = shaderc_env_version_vulkan_1_0;
				rmkl::Shader::s_spirvVersion = shaderc_spirv_version_1_0;
			}
			else if (v == 1.1f)
			{
				console.print("Compiling shaders for Vulkan 1.1\n\n", rmkl::ColoredConsole::Color::White);
				rmkl::Shader::s_vulkanVersion = shaderc_env_version_vulkan_1_1;
				rmkl::Shader::s_spirvVersion = shaderc_spirv_version_1_3;
			}
			else if (v == 1.2f)
			{
				console.print("Compiling shaders for Vulkan 1.2\n\n", rmkl::ColoredConsole::Color::White);
				rmkl::Shader::s_vulkanVersion = shaderc_env_version_vulkan_1_2;
				rmkl::Shader::s_spirvVersion = shaderc_spirv_version_1_5;
			}
			else
			{
				console.print("Invalid version specified", rmkl::ColoredConsole::Color::Red);
				return -1;
			}
		}
		else
		{
			console.print("Version missing.", rmkl::ColoredConsole::Color::Red);
		}

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
