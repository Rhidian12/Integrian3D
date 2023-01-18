#pragma once

#include "../../EngineConstants.h"
#include "../../Component/Component.h"

#include <glm.hpp> /* GLM */

namespace Integrian3D
{
	class CameraComponent final : public Component
	{
	public:
		CameraComponent(GameObject* pOwner, const float nearPlane, const float farPlane, const float fov,
			const float aspectRatio, const double speed = 10.0);

		virtual void Start() override;
		virtual void Update() override;

		void SetView(const glm::mat4& view);
		void SetSpeed(const float speed);

		__NODISCARD double GetSpeed() const { return m_Speed; }
		__NODISCARD const glm::mat4& GetView() const { return m_View; }
		__NODISCARD const glm::mat4& GetProjection() const { return m_Projection; }

	private:
		void HandleTranslation();
		void HandleRotation();

		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;
		float m_AspectRatio;
		double m_Speed;

		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
}