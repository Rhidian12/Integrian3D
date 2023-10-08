#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "Math/Math.h"
#include "Memory/UniquePtr.h"
#include "Shader/Shader.h"

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

	private:
		TArray<Texture*> Textures;
		Shader MaterialShader;
	};
}