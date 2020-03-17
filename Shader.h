#pragma once

#include <json.hpp>
#include <shaderc.hpp>
#include <filesystem>
#include "ColoredConsole.h"

namespace rmkl {

	class Shader
	{
	public:
		enum class Type { Vertex, Fragment, Compute };

	public:
		Shader(const nlohmann::json& data, Type type);
		void compile(shaderc::Compiler& compiler, const ColoredConsole& console, bool printEverything = false) const;

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
		std::string suffix(Type type) const;

	private:
		const Type type;
		std::filesystem::path input;
		std::vector<Config> configs;
	};
}