#pragma once

#include "EngineConstants.h"
#include "Array/Array.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Math/Math.h"
#include "Memory/UniquePtr.h"
#include "Shader/Shader.h"
#include "TPair/TPair.h"

#include <functional>
#include <map>
#include <string>
#include <string_view>

namespace Integrian3D
{
	class Light;
	class Texture;

	enum class TextureSlots : int32
	{
		Diffuse = 0,
		Specular = 1
	};

	class Material
	{
	public:
		Material(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);
		~Material();

		void AddTexture(const TextureSlots TextureSlot, Texture* const Texture);

		void StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection,
			const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights) const;

		void SetBool(const std::string_view Name, const bool Value);
		void SetInt(const std::string_view Name, const int Value);
		void SetFloat(const std::string_view Name, const float Value);
		void SetMatrix(const std::string_view Name, const Math::Mat4D& Value);
		void SetVec3(const std::string_view Name, const Math::Vec3D& Value);

	private:
		std::map<TextureSlots, Texture*> Textures;
		TArray<std::function<void()>> SetShaderVarRequests;
		Shader MaterialShader;
	};
}