#include "TransformComponent.h"

namespace Integrian3D
{
	TransformComponent::TransformComponent()
		: Transformation{}
		, LocalScale{ 1.f, 1.f }
	{
		const Matrix3f translationMatrix{ Matrix3f::MakeIdentityMatrix() };

		const Matrix3f rotationMatrix{ Matrix3f::MakeIdentityMatrix() };

		Matrix3f scaleMatrix{ Matrix3f::MakeIdentityMatrix() };
		scaleMatrix(0, 0) = LocalScale.X;
		scaleMatrix(1, 1) = LocalScale.Y;

		Transformation = translationMatrix * rotationMatrix * scaleMatrix;
	}
}