#include "TransformComponent.h"

#include "../../Math/MathUtils.h"

#include <gtc/matrix_transform.hpp>

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

	void TransformComponent::Translate(const glm::vec3& v)
	{
		Transformation[3] += glm::vec4{ v, 0.f };

		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::Rotate(const glm::vec3& axis, const float angleRad)
	{
		if (MathUtils::AreEqual(axis.x, 1.f))
		{
			LocalAngle.x += angleRad;
		}
		if (MathUtils::AreEqual(axis.y, 1.f))
		{
			LocalAngle.y += angleRad;
		}
		if (MathUtils::AreEqual(axis.z, 1.f))
		{
			LocalAngle.z += angleRad;
		}

		bShouldRecalculateTransform = true;
		bShouldRecalculateWorldData = true;
	}

	void TransformComponent::Scale(const glm::vec3& v)
	{
		/// LocalScale
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