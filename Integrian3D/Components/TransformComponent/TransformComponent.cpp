#include "TransformComponent.h"

namespace Integrian3D
{
	TransformComponent::TransformComponent()
		: Transformation{}
		, WorldLocation{}
		, WorldScale{ 1.f, 1.f, 1.f }
		, LocalScale{ 1.f, 1.f, 1.f }
		, WorldAngle{}
		, LocalAngle{}
		, bShouldRecalculateTransform{}
	{
		const Matrix4f translationMatrix{};

		const Matrix4f rotationMatrix{ Matrix4f::MakeRotationMatrix(LocalAngle.X, LocalAngle.Y, LocalAngle.Z) };

		Matrix4f scaleMatrix{ Matrix4f::MakeScaleMatrix(LocalScale.X, LocalScale.Y, LocalScale.Z) };

		Transformation = translationMatrix * rotationMatrix * scaleMatrix;
	}
}