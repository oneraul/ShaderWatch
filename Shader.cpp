#include "Shader.h"

namespace rmkl {

	static const std::string ROOT = "C:\\dev\\vkrenderer\\VkRenderer\\res\\shaders";

	Shader::Config::Config(std::string fileName, std::vector<std::string> defines = {})
		: output(ROOT)
	{
		output.append("compiled");
		output.append(fileName);
		this->defines.insert(this->defines.end(), defines.begin(), defines.end());
	}

	bool Shader::Config::upToDate(std::filesystem::path source) const
	{
		if (!std::filesystem::exists(output))
			return false;

		auto dateInput = std::filesystem::last_write_time(source);
		auto dateOutput = std::filesystem::last_write_time(output);
		return dateOutput.time_since_epoch() > dateInput.time_since_epoch();
	}

	Shader::Shader(const nlohmann::json& data, std::string suffix)
		: input(ROOT)
	{
		if (data.is_string())
		{
			std::string name = data;
			input.append(name + "." + suffix);
			configs.emplace_back(name + "_" + suffix + ".spv");
		}
		else
		{
			input.append(static_cast<std::string>(data["name"]) + "." + suffix);
			for (auto& variant : data["variants"])
			{
				std::string output = static_cast<std::string>(variant["outputFile"]);
				std::vector<std::string> defines;
				for (auto define : variant["defines"])
					defines.push_back(define);

				configs.emplace_back(output, defines);
			}
		}

	}

	inline bool Shader::exists() const
	{
		return std::filesystem::exists(input);
	}

	void Shader::compile(const ColoredConsole& console, bool printEverything) const
	{
		if (!exists())
		{
			if (printEverything)
				console.print("Not found: " + input.filename().string() + "\n", ColoredConsole::Color::Red);

			return;
		}

		for (auto& config : configs)
		{
			if (config.upToDate(input))
			{
				if (printEverything)
					console.print("Up to date: " + config.output.filename().string() + "\n", ColoredConsole::Color::Grey);
			}
			else
			{
				console.print("Compiling: " + config.output.filename().string() + "\n", ColoredConsole::Color::Green);
			}
		}
	}
}