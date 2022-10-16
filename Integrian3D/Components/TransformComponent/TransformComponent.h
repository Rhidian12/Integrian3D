#pragma once

#include "../../EngineConstants.h"
#include "../../Math/MathUtils.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class TransformComponent final
	{
	public:
		TransformComponent();

		TransformComponent(const TransformComponent&) noexcept = delete;
		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent&) noexcept = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept;

		void RecalculateTransform();

		void Translate(const MathUtils::Vec3D& v, const bool bForce = false);
		void Rotate(const MathUtils::Vec3D& rotationRad, const bool bForce = false);
		void Scale(const MathUtils::Vec3D& v, const bool bForce = false);

		void SetLocalLocation(const MathUtils::Vec3D& pos);
		void SetLocalScale(const MathUtils::Vec3D& scale, bool bForce = false);
		void SetLocalAngle(const MathUtils::Vec3D& angleRad, bool bForce = false);

		void SetForward(const MathUtils::Vec3D& forward);

		__NODISCARD const MathUtils::Vec3D& GetWorldLocation() const { return WorldLocation; }
		__NODISCARD MathUtils::Vec3D GetLocalLocation() const { return Transformation[3]; }

		__NODISCARD const MathUtils::Vec3D& GetWorldScale() const { return WorldScale; }
		__NODISCARD const MathUtils::Vec3D& GetLocalScale() const { return LocalScale; }

		__NODISCARD const MathUtils::Vec3D& GetWorldAngle() const { return WorldAngle; }
		__NODISCARD const MathUtils::Vec3D& GetLocalAngle() const { return LocalAngle; }

		__NODISCARD const MathUtils::Vec3D& GetForward() const { return Forward; };
		__NODISCARD const MathUtils::Vec3D& GetRight() const { return Right; };
		__NODISCARD const MathUtils::Vec3D& GetUp() const { return Up; };

		MathUtils::Mat4D Transformation;

		bool bShouldRecalculateTransform;
		bool bShouldRecalculateWorldData;

	private:
		void RecalculateDirectionVectors();

		MathUtils::Vec3D WorldLocation;

		MathUtils::Vec3D WorldScale;
		MathUtils::Vec3D LocalScale;

		MathUtils::Vec3D WorldAngle; /* In Radians */
		MathUtils::Vec3D LocalAngle; /* In Radians */

		MathUtils::Vec3D Forward;
		MathUtils::Vec3D Right;
		MathUtils::Vec3D Up;
	};
}