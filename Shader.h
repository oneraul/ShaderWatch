#pragma once

#include <json.hpp>
#include <shaderc.hpp>
#include <filesystem>
#include <optional>
#include "ColoredConsole.h"

namespace rmkl {

	class Shader
	{
	public:
		enum class Type { Vertex, Fragment, Compute };

	public:
		Shader(const nlohmann::json& data, Type type);
		void compile(shaderc::Compiler& compiler, const ColoredConsole& console, bool printEverything = false);

	private:
		struct Config
		{
			Config(std::string fileName, std::vector<std::string> defines);
			bool upToDate(std::filesystem::path source) const;

			std::filesystem::path output;
			std::vector<std::string> defines;
			shaderc::CompileOptions compileOptions;
		};

		struct Source
		{
			std::string code;
			std::filesystem::file_time_type lastWriteTime;
			bool compiled = false;
			//std::chrono::time_point<std::chrono::high_resolution_clock> lastCompilationTime;
		};

	private:
		inline bool exists() const;
		std::string suffix() const;
		shaderc_shader_kind shadercKind() const;
		std::optional<Shader::Source> getSource();

	private:
		const Type m_type;
		std::filesystem::path input;
		std::vector<Config> configs;
		std::optional<Source> m_source;
	};
}