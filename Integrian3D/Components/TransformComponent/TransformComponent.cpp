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

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept
		: Transformation{ std::move(other.Transformation) }
		, WorldLocation{ std::move(other.WorldLocation) }
		, WorldScale{ std::move(other.WorldScale) }
		, LocalScale{ std::move(other.LocalScale) }
		, WorldAngle{ std::move(other.WorldAngle) }
		, LocalAngle{ std::move(other.LocalAngle) }
		, bShouldRecalculateTransform{ std::move(other.bShouldRecalculateTransform) }
		, bShouldRecalculateWorldData{ std::move(other.bShouldRecalculateWorldData) }
	{
		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;
	}

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
	{
		Transformation = std::move(other.Transformation);
		WorldLocation = std::move(other.WorldLocation);
		WorldScale = std::move(other.WorldScale);
		LocalScale = std::move(other.LocalScale);
		WorldAngle = std::move(other.WorldAngle);
		LocalAngle = std::move(other.LocalAngle);
		bShouldRecalculateTransform = std::move(other.bShouldRecalculateTransform);
		bShouldRecalculateWorldData = std::move(other.bShouldRecalculateWorldData);

		other.bShouldRecalculateTransform = false;
		other.bShouldRecalculateWorldData = false;

		return *this;
	}

	void TransformComponent::Translate(const glm::vec3& v)
	{
		Transformation[3] += glm::vec4{ v, 0.f };

		bShouldRecalculateWorldData = true;
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