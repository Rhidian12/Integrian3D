#include "TransformComponent.h"

#include "../../Math/MathUtils.h"

#include <gtc/matrix_transform.hpp>
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

		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;

		return *this;
	}

	void TransformComponent::RecalculateTransform(bool bForce)
	{
		if (bShouldRecalculateTransform || bForce)
		{
			const glm::mat4 rotationMat{ glm::eulerAngleXYZ(LocalAngle.x, LocalAngle.y, LocalAngle.z) };

			glm::mat4 translationMat{ glm::identity<glm::mat4>() };
			translationMat[3] = Transformation[3];

			glm::mat4 scaleMat{ glm::identity<glm::mat4>() };
			scaleMat[0][0] = LocalScale.x;
			scaleMat[1][1] = LocalScale.y;
			scaleMat[2][2] = LocalScale.z;

			Transformation = translationMat * rotationMat * scaleMat;

			bShouldRecalculateTransform = false;
		}
	}

	void TransformComponent::Translate(const glm::vec3& v, bool bForce)
	{
		Transformation[3] += glm::vec4{ v, 0.f };

		bShouldRecalculateWorldData = true;

		if (bForce)
		{
			RecalculateTransform(bForce);
		}
	}

	void TransformComponent::Rotate(const glm::vec3& rotationRad)
	{
		LocalAngle.x += rotationRad.x;
		LocalAngle.y += rotationRad.y;
		LocalAngle.z += rotationRad.z;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::Scale(const glm::vec3& v)
	{
		LocalScale += v;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalLocation(const glm::vec3& pos)
	{
		Transformation[3] = glm::vec4{ pos, 1.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalScale(const glm::vec3& scale)
	{
		LocalScale = scale;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::SetLocalAngle(const glm::vec3& angleRad)
	{
		LocalAngle = angleRad;

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}
}