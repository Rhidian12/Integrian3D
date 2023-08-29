#pragma once

#include "../../EngineConstants.h"

#include "../../Component/Component.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class TransformComponent;

	// Can be inherited from
	class FreeCameraComponent : public Component
	{
	public:
		FreeCameraComponent(const float nearPlane, const float farPlane, const float fov,
			const float aspectRatio, const double speed = 10.0);
		virtual ~FreeCameraComponent() = default;

		void UpdateView(const TransformComponent& transform);
		void UpdateTranslation(TransformComponent& transform);
		void UpdateRotation(TransformComponent& transform);

		void SetSpeed(const double speed) { m_Speed = speed; }

		__NODISCARD __INLINE double GetSpeed() const { return m_Speed; }
		__NODISCARD __INLINE const glm::mat4& GetView() const { return m_View; }
		__NODISCARD __INLINE const glm::mat4& GetProjection() const { return m_Projection; }

		bool operator==(const FreeCameraComponent& other) const;

	private:
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;
		float m_AspectRatio;
		double m_Speed;

		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
}