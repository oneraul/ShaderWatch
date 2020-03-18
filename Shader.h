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
			shaderc::CompileOptions compileOptions;
		};

	private:
		inline bool exists() const;
		std::string suffix() const;
		shaderc_shader_kind shadercKind() const;

	private:
		const Type m_type;
		std::filesystem::path input;
		std::string m_source;
		std::vector<Config> configs;
	};
}