#pragma once

#include "../../EngineConstants.h"
#include "../../Math/Math.h"
#include "../../Component/Component.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* pOwner);

		// TransformComponent(const TransformComponent&) noexcept = delete;
		// TransformComponent(TransformComponent&& other) noexcept;
		// TransformComponent& operator=(const TransformComponent&) noexcept = delete;
		// TransformComponent& operator=(TransformComponent&& other) noexcept;

		/* [TODO]: Do in FixedUpdate */
		virtual void Update() override;

		void Translate(const Math::Vec3D& v, const bool bForce = false);
		void Rotate(const Math::Vec3D& rotationRad, const bool bForce = false);
		void Scale(const Math::Vec3D& v, const bool bForce = false);

		void SetLocalLocation(const Math::Vec3D& pos);
		void SetLocalScale(const Math::Vec3D& scale, bool bForce = false);
		void SetLocalAngle(const Math::Vec3D& angleRad, bool bForce = false);

		void SetForward(const Math::Vec3D& forward);

		__NODISCARD const Math::Vec3D& GetWorldLocation() const { return WorldLocation; }
		__NODISCARD Math::Vec3D GetLocalLocation() const { return Transformation[3]; }

		__NODISCARD const Math::Vec3D& GetWorldScale() const { return WorldScale; }
		__NODISCARD const Math::Vec3D& GetLocalScale() const { return LocalScale; }

		__NODISCARD const Math::Vec3D& GetWorldAngle() const { return WorldAngle; }
		__NODISCARD const Math::Vec3D& GetLocalAngle() const { return LocalAngle; }

		__NODISCARD const Math::Vec3D& GetForward() const { return Forward; };
		__NODISCARD const Math::Vec3D& GetRight() const { return Right; };
		__NODISCARD const Math::Vec3D& GetUp() const { return Up; };

		Math::Mat4D Transformation;

		bool bShouldRecalculateTransform;
		bool bShouldRecalculateWorldData;

	private:
		void RecalculateTransform();
		void RecalculateDirectionVectors();

		Math::Vec3D WorldLocation;

		Math::Vec3D WorldScale;
		Math::Vec3D LocalScale;

		Math::Vec3D WorldAngle; /* In Radians */
		Math::Vec3D LocalAngle; /* In Radians */

		Math::Vec3D Forward;
		Math::Vec3D Right;
		Math::Vec3D Up;
	};
}