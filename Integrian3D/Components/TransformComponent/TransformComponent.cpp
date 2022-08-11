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
		, bShouldRecalculateWorldData{}
	{
		const Matrix4f translationMatrix{};

		const Matrix4f rotationMatrix{ Matrix4f::MakeRotationMatrix(LocalAngle.X, LocalAngle.Y, LocalAngle.Z) };

		Matrix4f scaleMatrix{ Matrix4f::MakeScaleMatrix(LocalScale.X, LocalScale.Y, LocalScale.Z) };

		Transformation = translationMatrix * rotationMatrix * scaleMatrix;
	}

	void TransformComponent::SetTransform(const Matrix4f& transform)
	{
		Transformation = transform;
	}

	void TransformComponent::SetLocalLocation(const Point3f& pos)
	{
		Transformation(0, 3) = pos.X;
		Transformation(1, 3) = pos.Y;
		Transformation(2, 3) = pos.Z;

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const Point3f& scale)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}
	
	void TransformComponent::SetLocalAngle(const Point3f& angleRad)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}
}