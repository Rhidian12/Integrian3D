#pragma once

#include "EngineConstants.h"
#include "Component/Component.h"
#include "Math/Math.h"
#include "Memory/UniquePtr.h"

namespace Integrian3D
{
	class TransformComponent;

	// Can be inherited from
	class FreeCameraComponent : public Component
	{
	public:
		explicit FreeCameraComponent(const float FOV, const double Speed);
		virtual ~FreeCameraComponent() = default;

		void UpdateTranslation(TransformComponent& transform);
		void UpdateRotation(TransformComponent& transform);

		void SetSpeed(const double NewSpeed);

		__NODISCARD double GetSpeed() const;
		__NODISCARD const struct Camera3D* GetRayLibCamera() const;

	private:
		UniquePtr<struct Camera3D> CameraImpl;
		double Speed;
	};
}