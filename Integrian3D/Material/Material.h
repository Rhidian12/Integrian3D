#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "Math/Math.h"
#include "Memory/UniquePtr.h"
#include "Shader/Shader.h"

#include <functional>
#include <string>
#include <string_view>

namespace Integrian3D
{
	class Texture;

	class Material
	{
	public:
		Material(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);
		~Material();

		void AddTexture(Texture* const Texture);

		void StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection) const;

		void SetBool(const std::string_view Name, const bool Value);
		void SetInt(const std::string_view Name, const int Value);
		void SetFloat(const std::string_view Name, const float Value);
		void SetMatrix(const std::string_view Name, const Math::Mat4D& Value);
		void SetVec3(const std::string_view Name, const Math::Vec3D& Value);

	private:
		TArray<Texture*> Textures;
		TArray<std::function<void()>> SetShaderVarRequests;
		Shader MaterialShader;
	};
}