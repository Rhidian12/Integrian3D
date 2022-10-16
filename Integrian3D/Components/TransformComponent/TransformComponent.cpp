#include "TransformComponent.h"

#include "../../Math/MathUtils.h"

#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <gtx/euler_angles.hpp>
#pragma warning( pop )

namespace Integrian3D
{
	TransformComponent::TransformComponent()
		: Transformation{ MathUtils::Mat4D{ 1.f } /* Identity matrix */ }
		, WorldLocation{}
		, WorldScale{ 1.0, 1.0, 1.0 }
		, LocalScale{ 1.0, 1.0, 1.0 }
		, WorldAngle{}
		, LocalAngle{}
		, bShouldRecalculateTransform{}
		, bShouldRecalculateWorldData{}
		, Forward{ 0.0, 0.0, 1.0 }
		, Right{ 1.0, 0.0, 0.0 }
		, Up{ 0.0, 1.0, 0.0 }
	{}

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept
		: Transformation{ __MOVE(MathUtils::Mat4D, other.Transformation) }
		, WorldLocation{ __MOVE(MathUtils::Vec3D, other.WorldLocation) }
		, WorldScale{ __MOVE(MathUtils::Vec3D, other.WorldScale) }
		, LocalScale{ __MOVE(MathUtils::Vec3D, other.LocalScale) }
		, WorldAngle{ __MOVE(MathUtils::Vec3D, other.WorldAngle) }
		, LocalAngle{ __MOVE(MathUtils::Vec3D, other.LocalAngle) }
		, bShouldRecalculateTransform{ __MOVE(bool, other.bShouldRecalculateTransform) }
		, bShouldRecalculateWorldData{ __MOVE(bool, other.bShouldRecalculateWorldData) }
		, Forward{ __MOVE(MathUtils::Vec3D, other.Forward) }
		, Right{ __MOVE(MathUtils::Vec3D, other.Right) }
		, Up{ __MOVE(MathUtils::Vec3D, other.Up) }
	{
		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;
	}

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
	{
		Transformation = __MOVE(MathUtils::Mat4D, other.Transformation);
		WorldLocation = __MOVE(MathUtils::Vec3D, other.WorldLocation);
		WorldScale = __MOVE(MathUtils::Vec3D, other.WorldScale);
		LocalScale = __MOVE(MathUtils::Vec3D, other.LocalScale);
		WorldAngle = __MOVE(MathUtils::Vec3D, other.WorldAngle);
		LocalAngle = __MOVE(MathUtils::Vec3D, other.LocalAngle);
		bShouldRecalculateTransform = __MOVE(bool, other.bShouldRecalculateTransform);
		bShouldRecalculateWorldData = __MOVE(bool, other.bShouldRecalculateWorldData);
		Forward = __MOVE(MathUtils::Vec3D, other.Forward);
		Right = __MOVE(MathUtils::Vec3D, other.Right);
		Up = __MOVE(MathUtils::Vec3D, other.Up);

		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;

		return *this;
	}

	void TransformComponent::RecalculateTransform()
	{
		if (bShouldRecalculateTransform)
		{
			const MathUtils::Mat4D rotationMat{ glm::eulerAngleXYZ(LocalAngle.x, LocalAngle.y, LocalAngle.z) };

			MathUtils::Mat4D translationMat{ glm::identity<MathUtils::Mat4D>() };
			translationMat[3] = Transformation[3];

			MathUtils::Mat4D scaleMat{ glm::identity<MathUtils::Mat4D>() };
			scaleMat[0][0] = LocalScale.x;
			scaleMat[1][1] = LocalScale.y;
			scaleMat[2][2] = LocalScale.z;

			Transformation = translationMat * rotationMat * scaleMat;

			RecalculateDirectionVectors();

			bShouldRecalculateTransform = false;
		}
	}

	void TransformComponent::Translate(const MathUtils::Vec3D& v, const bool bForce)
	{
		Transformation[3] += MathUtils::Vec4D{ v, 0.f };

		bShouldRecalculateWorldData = true;
		bShouldRecalculateTransform = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::Rotate(const MathUtils::Vec3D& rotationRad, const bool bForce)
	{
		LocalAngle.x += rotationRad.x;
		LocalAngle.y += rotationRad.y;
		LocalAngle.z += rotationRad.z;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::Scale(const MathUtils::Vec3D& v, const bool bForce)
	{
		LocalScale += v;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalLocation(const MathUtils::Vec3D& pos)
	{
		Transformation[3] = MathUtils::Vec4D{ pos, 1.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const MathUtils::Vec3D& scale, bool bForce)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalAngle(const MathUtils::Vec3D& angleRad, bool bForce)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetForward(const MathUtils::Vec3D& forward)
	{
		Forward = forward;

		RecalculateTransform();
	}

	void TransformComponent::RecalculateDirectionVectors()
	{
		Forward = MathUtils::Vec3D{ 0.0, 0.0, 1.0 };

		Forward = glm::rotateX(Forward, LocalAngle.x);
		Forward = glm::rotateY(Forward, LocalAngle.y);
		Forward = glm::rotateZ(Forward, LocalAngle.z);

		Forward = glm::normalize(Forward);

		Right = glm::normalize(glm::cross(Forward, MathUtils::Up));
		Up = glm::normalize(glm::cross(Right, Forward));
	}
}