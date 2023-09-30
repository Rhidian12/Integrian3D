#pragma once

#include "EngineConstants.h"
#include "Component/Component.h"
#include "Math/Math.h"

struct Camera3D;
namespace Integrian3D
{
	class TransformComponent;

	// Can be inherited from
	class FreeCameraComponent : public Component
	{
	public:
		explicit FreeCameraComponent(const float FOV, const double Speed);
		virtual ~FreeCameraComponent();

		FreeCameraComponent(const FreeCameraComponent&) noexcept = delete;
		FreeCameraComponent(FreeCameraComponent&& Other) noexcept;
		FreeCameraComponent& operator=(const FreeCameraComponent&) noexcept = delete;
		FreeCameraComponent& operator=(FreeCameraComponent&& Other) noexcept;

		void UpdateTranslation(TransformComponent& transform);
		void UpdateRotation(TransformComponent& transform);

		void SetSpeed(const double NewSpeed);

		__NODISCARD double GetSpeed() const;
		__NODISCARD const Camera3D* const GetRayLibCamera() const;

		__NODISCARD bool operator==(const FreeCameraComponent& Other) const;

	private:
		Camera3D* CameraImpl; // By exception, this pointer IS owned by FreeCameraComponent
		double Speed;
	};
}