#pragma once

#include "EngineConstants.h"

#include "Component/Component.h"
#include "Math/Math.h"

namespace Integrian3D
{
	class TransformComponent;
	class Scene;

	// Can be inherited from
	class FreeCameraComponent : public Component
	{
	public:
		FreeCameraComponent();
		virtual ~FreeCameraComponent() = default;

		void Initialize(Scene* const Scene, const Entity Entity, const float nearPlane, const float farPlane, const float fov,
			const float aspectRatio, const float speed = 10.f);

		void UpdateView(const TransformComponent& transform);
		void UpdateTranslation(TransformComponent& transform);
		void UpdateRotation(TransformComponent& transform);

		void SetSpeed(const float speed);

		__NODISCARD __INLINE double GetSpeed() const { return m_Speed; }
		__NODISCARD __INLINE const Math::Mat4D& GetView() const { return m_View; }
		__NODISCARD __INLINE const Math::Mat4D& GetProjection() const { return m_Projection; }

		bool operator==(const FreeCameraComponent& other) const;

	private:
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;
		float m_AspectRatio;
		float m_Speed;

		Math::Mat4D m_View;
		Math::Mat4D m_Projection;
	};
}