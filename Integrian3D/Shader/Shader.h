#pragma once

#include "EngineConstants.h"
#include "IO/File/File.h"
#include "Math/Math.h"

#include <string> /* std::string */
#include <assert.h> /* assert() */
#include <string_view> /* std::string_view */

DEFINE_LOG_CATEGORY(ShaderLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	class Shader final
	{
		enum class ShaderType
		{
			VertexShader,
			FragmentShader
		};

	public:
		Shader(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);
		~Shader();

		Shader(const Shader&) noexcept = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader&) noexcept = delete;
		Shader& operator=(Shader&& other) noexcept;

		void Activate() const;
		void HotReloadShaders();

		void SetBool(const std::string_view name, const bool value) const;
		void SetInt(const std::string_view name, const int value) const;
		void SetFloat(const std::string_view name, const float value) const;
		void SetMatrix(const std::string_view name, const glm::mat4& value) const;
		void SetVec3(const std::string_view Name, const Math::Vec3D& Vec) const;

	private:
		void SetupShaders();
		void OnShaderChanged(const std::string& Filepath);
		void CompileShader(const ShaderType ShaderType);
		void CreateProgram();

		IO::File VertexShader;
		IO::File FragmentShader;
		uint32 ProgramID;
		uint32 VertexShaderID;
		uint32 FragmentShaderID;
		uint8 bReloadVertexShader : 1;
		uint8 bReloadFragmentShader : 1;
	};
}