#include "TestRotateComponent.h"

#include "../TransformComponent/TransformComponent.h"

#include "../../Timer/Timer.h"

namespace Integrian3D
{
	void TestRotateComponent::Rotate(TransformComponent& transform)
	{	
		transform.Rotate
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