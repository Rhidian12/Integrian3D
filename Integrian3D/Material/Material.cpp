#include "Material/Material.h"

#include "Light/Light.h"
#include "Texture/Texture.h"
#include "TPair/TPair.full.h"

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

			__CHECK(false);
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
		const Math::Vec3D& CameraPosition, const TArray<TPair<TransformComponent, Light*>>& Lights) const
	{
		MaterialShader.Activate();

		MaterialShader.SetMatrix("_View", View);

		MaterialShader.SetMatrix("_Projection", Projection);

		MaterialShader.SetMatrix("_Transform", Transform);

		MaterialShader.SetVec3("_ViewPos", CameraPosition);

		// [TODO]: Currently only supports 1 light
		if (!Lights.Empty())
		{
			const TPair<TransformComponent, Light*>& Pair{ Lights[0] };

			MaterialShader.SetVec3("_Light.Position", Pair.Key().GetLocalLocation());
			MaterialShader.SetVec3("_Light.Ambient", Pair.Value()->GetAmbient());
			MaterialShader.SetVec3("_Light.Diffuse", Pair.Value()->GetDiffuse());
			MaterialShader.SetVec3("_Light.Specular", Pair.Value()->GetSpecular());
		}

		for (const auto& Request : SetShaderVarRequests)
		{
			Request();
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
		// SetShaderVarRequests.Add([=]() { MaterialShader.SetBool(Name, Value); });
	}

	void Material::SetInt(const std::string_view Name, const int Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetInt(Name, Value);
		// SetShaderVarRequests.Add([=]() { MaterialShader.SetInt(Name, Value); });
	}

	void Material::SetFloat(const std::string_view Name, const float Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetFloat(Name, Value);
		// SetShaderVarRequests.Add([=]() { MaterialShader.SetFloat(Name, Value); });
	}

	void Material::SetMatrix(const std::string_view Name, const Math::Mat4D& Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetMatrix(Name, Value);
		// SetShaderVarRequests.Add([=]() { MaterialShader.SetMatrix(Name, Value); });
	}

	void Material::SetVec3(const std::string_view Name, const Math::Vec3D& Value)
	{
		MaterialShader.Activate();

		MaterialShader.SetVec3(Name, Value);
		// SetShaderVarRequests.Add([=]() { MaterialShader.SetVec3(Name, Value); });
	}
}
