#include "Shader.h"
#include <fstream>
#include <sstream>

namespace rmkl {

	//static const std::string ROOT = "C:\\dev\\vkrenderer\\VkRenderer\\res\\shaders";
	static const std::string ROOT = "C:\\Users\\oneraul\\Desktop\\shaders";

	Shader::Config::Config(std::string fileName, std::vector<std::string> defines = {})
		: output(ROOT)
	{
		output.append("compiled");
		output.append(fileName);
		this->defines.insert(this->defines.end(), defines.begin(), defines.end());

		compileOptions.SetTargetSpirv(shaderc_spirv_version_1_3);
		compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		for (const auto& define : defines)
			compileOptions.AddMacroDefinition(define);
	}

	bool Shader::Config::upToDate(std::filesystem::path source) const
	{
		if (!std::filesystem::exists(output))
			return false;

		auto dateInput = std::filesystem::last_write_time(source);
		auto dateOutput = std::filesystem::last_write_time(output);
		return dateOutput.time_since_epoch() > dateInput.time_since_epoch();
	}

	Shader::Shader(const nlohmann::json& data, Type type)
		: input(ROOT)
		, m_type(type)
	{
		if (data.is_string())
		{
			std::string name = data;
			input.append(name + "." + suffix());
			configs.emplace_back(name + "_" + suffix() + ".spv");
		}
		else
		{
			input.append(static_cast<std::string>(data["name"]) + "." + suffix());
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

	std::string Shader::suffix() const
	{
		switch (m_type)
		{
		case rmkl::Shader::Type::Vertex:   return "vert";
		case rmkl::Shader::Type::Fragment: return "frag";
		case rmkl::Shader::Type::Compute:  return "comp";
		}
	}

	shaderc_shader_kind Shader::shadercKind() const
	{
		switch (m_type)
		{
		case rmkl::Shader::Type::Vertex:   return shaderc_vertex_shader;
		case rmkl::Shader::Type::Fragment: return shaderc_fragment_shader;
		case rmkl::Shader::Type::Compute:  return shaderc_compute_shader;
		}
	}

	std::optional<Shader::Source> Shader::getSource()
	{
		auto lastWriteTime = std::filesystem::last_write_time(input);
		if (!m_source || m_source->lastWriteTime < lastWriteTime)
		{
			std::ifstream ifs(input.string());
			std::stringstream sstr;
			while (ifs >> sstr.rdbuf());

			m_source = { sstr.str(), lastWriteTime };
		}

		return m_source;
	}

	void Shader::compile(shaderc::Compiler& compiler, const ColoredConsole& console, bool printEverything)
	{
		if (!exists())
		{
			if (printEverything)
				console.print("Not found: " + input.filename().string() + "\n", ColoredConsole::Color::Red);

			return;
		}

		if (auto source = getSource(); source && !source->compiled)
		{
			m_source->compiled = true;

			for (auto& config : configs)
			{
				if (config.upToDate(input))
				{
					if (printEverything)
						console.print("Up to date: " + config.output.filename().string() + "\n", ColoredConsole::Color::Grey);
				}
				else
				{
					const char* id = config.output.filename().string().c_str();
					shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source->code, shadercKind(), id, config.compileOptions);

					if (result.GetCompilationStatus() != shaderc_compilation_status_success)
					{
						console.print(config.output.filename().string() + "\n", ColoredConsole::Color::Red);
						console.print(result.GetErrorMessage() + "\n", ColoredConsole::Color::Grey);
						continue;
					}

					console.print(config.output.filename().string() + "\n\n", ColoredConsole::Color::Green);

					std::vector<uint32_t> spv;
					for (uint32_t v : result)
						spv.push_back(v);

					auto ofs = std::fstream(config.output.string(), std::ios::out | std::ios::binary);
					ofs.write((char*)spv.data(), spv.size() * sizeof(uint32_t));
					ofs.close();
				}
			}
		}
	}
}