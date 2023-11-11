#include "Material/Material.h"

#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"

#include "Texture/Texture.h"
#include "TPair/TPair.full.h"

#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Integrian3D
{
	namespace
	{
		static std::string_view GetStringFromTextureSlot(const TextureSlots TextureSlot)
		{
			switch (TextureSlot)
			{
				case TextureSlots::Diffuse:
				{
					constexpr static std::string_view Diffuse = "_Material.Diffuse";
					return Diffuse;
				}
				break;
				case TextureSlots::Specular:
				{
					constexpr static std::string_view Specular = "_Material.Specular";
					return Specular;
				}
				break;
			}

			IASSERT(false);
			return "";
		}

		static std::string GetLightName(int32& PointLightNr, int32& DirectionalLightNr, const Light* const Light)
		{
			switch (Light->GetLightType())
			{
				case LightType::PointLight:
				{
					constexpr static std::string_view PointLightName = "_PointLights[{}].";
					return std::vformat(PointLightName, std::make_format_args(PointLightNr++));
				}
				break;
				case LightType::DirectionalLight:
				{
					constexpr static std::string_view DirectionalLightName = "_DirectionalLights[{}].";
					return std::vformat(DirectionalLightName, std::make_format_args(DirectionalLightNr++));
				}
				break;
			}

			IASSERT(false);
			return "";
		}
	}

	Material::Material(const std::string& VertexShaderPath, const std::string& FragmentShaderPath)
		: Textures{}
		, MaterialShader{ VertexShaderPath, FragmentShaderPath }
	{}

	Material::~Material() {}

	void Material::AddTexture(const TextureSlots TextureSlot, Texture* const Texture)
	{
		Textures.insert(std::make_pair(TextureSlot, Texture));
	}

	void Material::StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection,
		const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights)
	{
		MaterialShader.HotReloadShaders();

		MaterialShader.Activate();

		MaterialShader.SetMatrix("_View", View);

		MaterialShader.SetMatrix("_Projection", Projection);

		MaterialShader.SetMatrix("_Transform", Transform);

		MaterialShader.SetVec3("_ViewPos", CameraPosition);

		// [TODO]: Fragment shader only supports 1 light
		int32 PointLightNr{}, DirectionalLightNr{};
		for (const auto& [LightTransform, Light] : Lights)
		{
			const std::string LightName{ GetLightName(PointLightNr, DirectionalLightNr, Light) };

			MaterialShader.SetVec3(LightName + "Ambient", Light->GetAmbient());
			MaterialShader.SetVec3(LightName + "Diffuse", Light->GetDiffuse());
			MaterialShader.SetVec3(LightName + "Specular", Light->GetSpecular());

			switch (Light->GetLightType())
			{
				case LightType::PointLight:
					MaterialShader.SetVec3(LightName + "Position", LightTransform.GetLocalLocation());
					MaterialShader.SetFloat(LightName + "MaxRadius", static_cast<PointLight*>(Light)->GetMaxRadius());
					MaterialShader.SetFloat(LightName + "Intensity", static_cast<PointLight*>(Light)->GetIntensity());
					break;
				case LightType::DirectionalLight:
					MaterialShader.SetVec3(LightName + "Direction", static_cast<DirectionalLight*>(Light)->GetDirection());
					break;
			}
		}

		for (const auto& [TextureSlot, Texture] : Textures)
		{
			MaterialShader.SetInt(GetStringFromTextureSlot(TextureSlot), static_cast<int32>(TextureSlot));

			glActiveTexture(GL_TEXTURE0 + static_cast<int32>(TextureSlot));

			glBindTexture(GL_TEXTURE_2D, Texture->GetTextureID());
		}
	}

	void Material::SetBool(const std::string_view Name, const bool Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetBool(Name, Value);
	}

	void Material::SetInt(const std::string_view Name, const int Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetInt(Name, Value);
	}

	void Material::SetFloat(const std::string_view Name, const float Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetFloat(Name, Value);
	}

	void Material::SetMatrix(const std::string_view Name, const Math::Mat4D& Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetMatrix(Name, Value);
	}

	void Material::SetVec3(const std::string_view Name, const Math::Vec3D& Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetVec3(Name, Value);
	}
}
