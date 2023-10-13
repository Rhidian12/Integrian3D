#include "Material/Material.h"
#include "Texture/Texture.h"

namespace Integrian3D
{
	Material::Material(const std::string& VertexShaderPath, const std::string& FragmentShaderPath)
		: Textures{}
		, MaterialShader{ VertexShaderPath, FragmentShaderPath }
	{}

	Material::~Material() {}

	void Material::AddTexture(Texture* const Texture)
	{
		Textures.Add(Texture);
	}

	void Material::StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection, const Math::Vec3D& CameraPosition) const
	{
		MaterialShader.Activate();

		MaterialShader.SetMatrix("_View", View);

		MaterialShader.SetMatrix("_Projection", Projection);

		MaterialShader.SetMatrix("_Transform", Transform);

		MaterialShader.SetVec3("_ViewPos", CameraPosition);

		for (const auto& Request : SetShaderVarRequests)
		{
			Request();
		}

		if (Textures.Size() > 0)
		{
			// [TODO]: Currently only able to process 1 texture
			MaterialShader.SetInt("_Material.Diffuse", 0);

			glActiveTexture(GL_TEXTURE0);

			glBindTexture(GL_TEXTURE_2D, Textures[0]->GetTextureID());
		}
	}

	void Material::SetBool(const std::string_view Name, const bool Value)
	{
		SetShaderVarRequests.Add([=]() { MaterialShader.SetBool(Name, Value); });
	}

	void Material::SetInt(const std::string_view Name, const int Value)
	{
		SetShaderVarRequests.Add([=]() { MaterialShader.SetInt(Name, Value); });
	}

	void Material::SetFloat(const std::string_view Name, const float Value)
	{
		SetShaderVarRequests.Add([=]() { MaterialShader.SetFloat(Name, Value); });
	}

	void Material::SetMatrix(const std::string_view Name, const Math::Mat4D& Value)
	{
		SetShaderVarRequests.Add([=]() { MaterialShader.SetMatrix(Name, Value); });
	}

	void Material::SetVec3(const std::string_view Name, const Math::Vec3D& Value)
	{
		SetShaderVarRequests.Add([=]() { MaterialShader.SetVec3(Name, Value); });
	}
}
