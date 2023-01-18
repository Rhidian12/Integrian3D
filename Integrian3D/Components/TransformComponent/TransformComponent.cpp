#include "TransformComponent.h"

#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <gtx/euler_angles.hpp>
#pragma warning( pop )

namespace Integrian3D
{
	TransformComponent::TransformComponent(GameObject* pOwner)
		: Component{ pOwner }
		, Transformation{ Math::Mat4D{ 1.f } /* Identity matrix */ }
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

	//TransformComponent::TransformComponent(TransformComponent&& other) noexcept
	//	: Transformation{ __MOVE(other.Transformation) }
	//	, WorldLocation{ __MOVE(other.WorldLocation) }
	//	, WorldScale{ __MOVE(other.WorldScale) }
	//	, LocalScale{ __MOVE(other.LocalScale) }
	//	, WorldAngle{ __MOVE(other.WorldAngle) }
	//	, LocalAngle{ __MOVE(other.LocalAngle) }
	//	, bShouldRecalculateTransform{ __MOVE(other.bShouldRecalculateTransform) }
	//	, bShouldRecalculateWorldData{ __MOVE(other.bShouldRecalculateWorldData) }
	//	, Forward{ __MOVE(other.Forward) }
	//	, Right{ __MOVE(other.Right) }
	//	, Up{ __MOVE(other.Up) }
	//{
	//	other.bShouldRecalculateTransform = false;
	//	other.bShouldRecalculateWorldData = false;
	//}

	//TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
	//{
	//	Transformation = __MOVE(other.Transformation);
	//	WorldLocation = __MOVE(other.WorldLocation);
	//	WorldScale = __MOVE(other.WorldScale);
	//	LocalScale = __MOVE(other.LocalScale);
	//	WorldAngle = __MOVE(other.WorldAngle);
	//	LocalAngle = __MOVE(other.LocalAngle);
	//	bShouldRecalculateTransform = __MOVE(other.bShouldRecalculateTransform);
	//	bShouldRecalculateWorldData = __MOVE(other.bShouldRecalculateWorldData);
	//	Forward = __MOVE(other.Forward);
	//	Right = __MOVE(other.Right);
	//	Up = __MOVE(other.Up);

	//	other.bShouldRecalculateTransform = false;
	//	other.bShouldRecalculateWorldData = false;

	//	return *this;
	//}

	void TransformComponent::Update()
	{
		RecalculateTransform();
	}

	void TransformComponent::RecalculateTransform()
	{
		if (bShouldRecalculateTransform)
		{
			const Math::Mat4D rotationMat{ glm::eulerAngleXYZ(LocalAngle.x, LocalAngle.y, LocalAngle.z) };

			Math::Mat4D translationMat{ glm::identity<Math::Mat4D>() };
			translationMat[3] = Transformation[3];

			Math::Mat4D scaleMat{ glm::identity<Math::Mat4D>() };
			scaleMat[0][0] = LocalScale.x;
			scaleMat[1][1] = LocalScale.y;
			scaleMat[2][2] = LocalScale.z;

			Transformation = translationMat * rotationMat * scaleMat;

			RecalculateDirectionVectors();

			bShouldRecalculateTransform = false;
		}
	}

	void TransformComponent::Translate(const Math::Vec3D& v, const bool bForce)
	{
		Transformation[3] += Math::Vec4D{ v, 0.f };

		bShouldRecalculateWorldData = true;
		bShouldRecalculateTransform = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::Rotate(const Math::Vec3D& rotationRad, const bool bForce)
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

	void TransformComponent::Scale(const Math::Vec3D& v, const bool bForce)
	{
		LocalScale += v;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalLocation(const Math::Vec3D& pos)
	{
		Transformation[3] = Math::Vec4D{ pos, 1.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const Math::Vec3D& scale, bool bForce)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalAngle(const Math::Vec3D& angleRad, bool bForce)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetForward(const Math::Vec3D& forward)
	{
		Forward = forward;

		RecalculateTransform();
	}

	void TransformComponent::RecalculateDirectionVectors()
	{
		Forward = Math::Vec3D{ 0.0, 0.0, 1.0 };

		Forward = glm::rotateX(Forward, LocalAngle.x);
		Forward = glm::rotateY(Forward, LocalAngle.y);
		Forward = glm::rotateZ(Forward, LocalAngle.z);

		Forward = glm::normalize(Forward);

		Right = glm::normalize(glm::cross(Forward, Math::Up));
		Up = glm::normalize(glm::cross(Right, Forward));
	}
}