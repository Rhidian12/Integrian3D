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

	void Material::StartShader(const Math::Mat4D& Transform, const Math::Mat4D& View, const Math::Mat4D& Projection) const
	{
		MaterialShader.Activate();

		MaterialShader.SetMatrix("_View", View);

		MaterialShader.SetMatrix("_Projection", Projection);

		MaterialShader.SetMatrix("_Transform", Transform);

		glActiveTexture(GL_TEXTURE0);

		// [TODO]: Currently only able to process 1 texture
		glBindTexture(GL_TEXTURE_2D, Textures[0]->GetTextureID());
	}
}
