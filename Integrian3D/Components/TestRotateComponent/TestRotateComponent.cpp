#include "TestRotateComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "../../Timer/Timer.h"

namespace Integrian3D
{
	TestRotateComponent::TestRotateComponent(GameObject* pOwner)
		: Component{ pOwner }
	{}

	void TestRotateComponent::Update()
	{	
		m_pOwner->pTransform->Rotate
		(
			Math::Vec3D
			{
				Math::ToRadians(25.f) * Time::Timer::GetInstance().GetElapsedSeconds(),
				Math::ToRadians(50.f) * Time::Timer::GetInstance().GetElapsedSeconds(),
				0.f
			}
		);
	}
}