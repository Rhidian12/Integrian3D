#include "TransformComponent.h"

#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <gtx/quaternion.hpp>
#pragma warning( pop )

namespace Integrian3D
{
	TransformComponent::TransformComponent()
		: Transformation{ Math::Mat4D{ 1.f } /* Identity matrix */ }
		, WorldLocation{}
		, WorldScale{ 1.0, 1.0, 1.0 }
		, LocalScale{ 1.0, 1.0, 1.0 }
		, WorldAngle{}
		, LocalAngle{ glm::quat_identity<float, glm::packed_highp>() }
		, bShouldRecalculateTransform{}
		, bShouldRecalculateWorldData{}
		, Forward{ 0.0, 0.0, 1.0 }
		, Right{ 1.0, 0.0, 0.0 }
		, Up{ 0.0, 1.0, 0.0 }
	{}

	//TransformComponent::TransformComponent(TransformComponent&& other) noexcept
	//	: Transformation{ I_MOVE(other.Transformation) }
	//	, WorldLocation{ I_MOVE(other.WorldLocation) }
	//	, WorldScale{ I_MOVE(other.WorldScale) }
	//	, LocalScale{ I_MOVE(other.LocalScale) }
	//	, WorldAngle{ I_MOVE(other.WorldAngle) }
	//	, LocalAngle{ I_MOVE(other.LocalAngle) }
	//	, bShouldRecalculateTransform{ I_MOVE(other.bShouldRecalculateTransform) }
	//	, bShouldRecalculateWorldData{ I_MOVE(other.bShouldRecalculateWorldData) }
	//	, Forward{ I_MOVE(other.Forward) }
	//	, Right{ I_MOVE(other.Right) }
	//	, Up{ I_MOVE(other.Up) }
	//{
	//	other.bShouldRecalculateTransform = false;
	//	other.bShouldRecalculateWorldData = false;
	//}

	//TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
	//{
	//	Transformation = I_MOVE(other.Transformation);
	//	WorldLocation = I_MOVE(other.WorldLocation);
	//	WorldScale = I_MOVE(other.WorldScale);
	//	LocalScale = I_MOVE(other.LocalScale);
	//	WorldAngle = I_MOVE(other.WorldAngle);
	//	LocalAngle = I_MOVE(other.LocalAngle);
	//	bShouldRecalculateTransform = I_MOVE(other.bShouldRecalculateTransform);
	//	bShouldRecalculateWorldData = I_MOVE(other.bShouldRecalculateWorldData);
	//	Forward = I_MOVE(other.Forward);
	//	Right = I_MOVE(other.Right);
	//	Up = I_MOVE(other.Up);

	//	other.bShouldRecalculateTransform = false;
	//	other.bShouldRecalculateWorldData = false;

	//	return *this;
	//}

	void TransformComponent::RecalculateTransform()
	{
		if (bShouldRecalculateTransform)
		{
			const Math::Mat4D rotationMat{ glm::toMat4(LocalAngle) };

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

	void TransformComponent::Translate(const Math::Vec3D& v)
	{
		Transformation[3] += Math::Vec4D{ v, 0.f };

		bShouldRecalculateWorldData = true;
		bShouldRecalculateTransform = true;
	}

	void TransformComponent::Rotate(const Math::Vec3D& rotationRad)
	{
		LocalAngle *= Math::Quat(rotationRad);

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::Scale(const Math::Vec3D& v)
	{
		LocalScale += v;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalLocation(const Math::Vec3D& pos)
	{
		Transformation[3] = Math::Vec4D{ pos, 1.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const Math::Vec3D& scale)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalAngle(const Math::Vec3D& angleRad)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetForward(const Math::Vec3D& forward)
	{
		Forward = forward;

		RecalculateTransform();
	}

	void TransformComponent::RecalculateDirectionVectors()
	{
		Forward = Math::Vec3D{ 0.0, 0.0, -1.0 };

		Forward = Forward * LocalAngle;

		Forward = glm::normalize(Forward);

		Right = glm::normalize(glm::cross(Forward, Math::Up));
		Up = glm::normalize(glm::cross(Right, Forward));
	}
}