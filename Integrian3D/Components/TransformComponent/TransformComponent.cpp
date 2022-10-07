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
		: Transformation{ glm::mat4{ 1.f } /* Identity matrix */ }
		, WorldLocation{}
		, WorldScale{ 1.f, 1.f, 1.f }
		, LocalScale{ 1.f, 1.f, 1.f }
		, WorldAngle{}
		, LocalAngle{}
		, bShouldRecalculateTransform{}
		, bShouldRecalculateWorldData{}
		, Forward{ 0.f, 0.f, 1.f }
		, Right{ 1.f, 0.f, 0.f }
		, Up{ 0.f, 1.f, 0.f }
	{}

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept
		: Transformation{ __MOVE(glm::mat4, other.Transformation) }
		, WorldLocation{ __MOVE(glm::vec3, other.WorldLocation) }
		, WorldScale{ __MOVE(glm::vec3, other.WorldScale) }
		, LocalScale{ __MOVE(glm::vec3, other.LocalScale) }
		, WorldAngle{ __MOVE(glm::vec3, other.WorldAngle) }
		, LocalAngle{ __MOVE(glm::vec3, other.LocalAngle) }
		, bShouldRecalculateTransform{ __MOVE(bool, other.bShouldRecalculateTransform) }
		, bShouldRecalculateWorldData{ __MOVE(bool, other.bShouldRecalculateWorldData) }
		, Forward{ __MOVE(glm::vec3, other.Forward) }
		, Right{ __MOVE(glm::vec3, other.Right) }
		, Up{ __MOVE(glm::vec3, other.Up) }
	{
		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;
	}

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
	{
		Transformation = __MOVE(glm::mat4, other.Transformation);
		WorldLocation = __MOVE(glm::vec3, other.WorldLocation);
		WorldScale = __MOVE(glm::vec3, other.WorldScale);
		LocalScale = __MOVE(glm::vec3, other.LocalScale);
		WorldAngle = __MOVE(glm::vec3, other.WorldAngle);
		LocalAngle = __MOVE(glm::vec3, other.LocalAngle);
		bShouldRecalculateTransform = __MOVE(bool, other.bShouldRecalculateTransform);
		bShouldRecalculateWorldData = __MOVE(bool, other.bShouldRecalculateWorldData);
		Forward = __MOVE(glm::vec3, other.Forward);
		Right = __MOVE(glm::vec3, other.Right);
		Up = __MOVE(glm::vec3, other.Up);

		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;

		return *this;
	}

	void TransformComponent::RecalculateTransform()
	{
		if (bShouldRecalculateTransform)
		{
			const glm::mat4 rotationMat{ glm::eulerAngleXYZ(LocalAngle.x, LocalAngle.y, LocalAngle.z) };

			glm::mat4 translationMat{ glm::identity<glm::mat4>() };
			translationMat[3] = Transformation[3];

			glm::mat4 scaleMat{ glm::identity<glm::mat4>() };
			scaleMat[0][0] = LocalScale.x;
			scaleMat[1][1] = LocalScale.y;
			scaleMat[2][2] = LocalScale.z;

			Transformation = translationMat * rotationMat * scaleMat;

			RecalculateDirectionVectors();

			bShouldRecalculateTransform = false;
		}
	}

	void TransformComponent::Translate(const glm::vec3& v, const bool bForce)
	{
		Transformation[3] += glm::vec4{ v, 0.f };

		bShouldRecalculateWorldData = true;
		bShouldRecalculateTransform = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::Rotate(const glm::vec3& rotationRad, const bool bForce)
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

	void TransformComponent::Scale(const glm::vec3& v, const bool bForce)
	{
		LocalScale += v;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalLocation(const glm::vec3& pos)
	{
		Transformation[3] = glm::vec4{ pos, 1.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const glm::vec3& scale, bool bForce)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetLocalAngle(const glm::vec3& angleRad, bool bForce)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform();
		}
	}

	void TransformComponent::SetForward(const glm::vec3& forward)
	{
		Forward = forward;

		RecalculateTransform();
	}

	void TransformComponent::RecalculateDirectionVectors()
	{
		Forward = glm::rotateX(Forward, LocalAngle.x);
		Forward = glm::rotateY(Forward, LocalAngle.y);
		Forward = glm::rotateZ(Forward, LocalAngle.z);

		Forward = glm::normalize(Forward);

		Right = glm::normalize(glm::cross(Forward, MathUtils::Up));
		Up = glm::normalize(glm::cross(Right, Forward));
	}
}