#pragma once

#include "EngineConstants.h"
#include "Math/Math.h"
#include "Component/Component.h"

namespace Integrian3D
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent();

		// TransformComponent(const TransformComponent&) noexcept = delete;
		// TransformComponent(TransformComponent&& other) noexcept;
		// TransformComponent& operator=(const TransformComponent&) noexcept = delete;
		// TransformComponent& operator=(TransformComponent&& other) noexcept;

		void RecalculateTransform();

		void Translate(const Math::Vec3D& v);
		void Rotate(const Math::Vec3D& rotationRad);
		void Scale(const Math::Vec3D& v);

		void SetLocalLocation(const Math::Vec3D& pos);
		void SetLocalScale(const Math::Vec3D& scale);
		void SetLocalAngle(const Math::Vec3D& angleRad);

		void SetForward(const Math::Vec3D& forward);

		__NODISCARD const Math::Vec3D& GetWorldLocation() const { return WorldLocation; }
		__NODISCARD Math::Vec3D GetLocalLocation() const { return Transformation[3]; }

		__NODISCARD const Math::Vec3D& GetWorldScale() const { return WorldScale; }
		__NODISCARD const Math::Vec3D& GetLocalScale() const { return LocalScale; }

		__NODISCARD const Math::Vec3D& GetWorldAngle() const { return WorldAngle; }
		__NODISCARD const Math::QuatD& GetLocalAngle() const { return LocalAngle; }

		__NODISCARD const Math::Vec3D& GetForward() const { return Forward; };
		__NODISCARD const Math::Vec3D& GetRight() const { return Right; };
		__NODISCARD const Math::Vec3D& GetUp() const { return Up; };

		Math::Mat4D Transformation;

		bool bShouldRecalculateTransform;
		bool bShouldRecalculateWorldData;

	private:
		void RecalculateDirectionVectors();

		Math::Vec3D WorldLocation;

		Math::Vec3D WorldScale;
		Math::Vec3D LocalScale;

		Math::Vec3D WorldAngle; /* In Radians */
		Math::QuatD LocalAngle; /* In Radians */

		Math::Vec3D Forward;
		Math::Vec3D Right;
		Math::Vec3D Up;
	};
}