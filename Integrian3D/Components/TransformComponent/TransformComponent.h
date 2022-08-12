#pragma once

#include <glm.hpp>

#include "../../EngineConstants.h"

namespace Integrian3D
{
	class TransformComponent final
	{
	public:
		TransformComponent();

		void Translate(const glm::vec3& v);
		void Rotate(const glm::vec3& axis, const float angleRad);
		void Scale(const glm::vec3& v);

		void SetLocalLocation(const glm::vec3& pos);
		void SetLocalScale(const glm::vec3& scale);
		void SetLocalAngle(const glm::vec3& angleRad);

		const glm::vec3& GetWorldLocation() const { return WorldLocation; }
		glm::vec3 GetLocalLocation() const { return Transformation[3]; }

		const glm::vec3& GetWorldScale() const { return WorldScale; }
		const glm::vec3& GetLocalScale() const { return LocalScale; }

		const glm::vec3& GetWorldAngle() const { return WorldAngle; }
		const glm::vec3& GetLocalAngle() const { return LocalAngle; }

		glm::mat4 Transformation;

		bool bShouldRecalculateTransform;
		bool bShouldRecalculateWorldData;

	private:
		glm::vec3 WorldLocation;

		glm::vec3 WorldScale;
		glm::vec3 LocalScale;

		glm::vec3 WorldAngle; /* In Radians */
		glm::vec3 LocalAngle; /* In Radians */
	};
}