#include "TransformComponent.h"

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

	void TransformComponent::Rotate(const glm::vec3& axis, const float angleRad)
	{
		Transformation = glm::rotate(Transformation, angleRad, axis);
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