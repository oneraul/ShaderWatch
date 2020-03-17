#pragma once

#include "json.hpp"
#include <filesystem>
#include "ColoredConsole.h"

class Shader
{
public:
	Shader(const nlohmann::json& data, std::string suffix);
	void compile(const ColoredConsole& console, bool printEverything = false) const;

private:
	struct Config
	{
		Config(std::string fileName, std::vector<std::string> defines);
		bool upToDate(std::filesystem::path source) const;

		std::filesystem::path output;
		std::vector<std::string> defines;
	};

private:
	inline bool exists() const;

private:
	std::filesystem::path input;
	std::vector<Config> configs;
};