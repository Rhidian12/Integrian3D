//#define SEED 0
#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "Timer/Timer.h"
#include "Systems/Systems.h"

int main()
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(1080,720) };

	Scene testScene{ "TestScene" };

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, "Resources/vehicle.obj", TextureManager::GetInstance().GetTexture("__Wall"));
	testScene.GetComponent<TransformComponent>(entity).SetLocalScale(Math::Vec3D{ 0.1,0.1,0.1 });

	//testScene.AddUpdateCallback([](Scene& scene)->void
	//	{
	//		scene.CreateView<TransformComponent, MeshComponent>().ForEach([](TransformComponent& transform, const MeshComponent&)->void
	//			{
	//				transform.Rotate
	//				(
	//					glm::vec3
	//					{
	//						Math::ToRadians(25.f) * Timer::GetInstance().GetElapsedSeconds(),
	//						Math::ToRadians(50.f) * Timer::GetInstance().GetElapsedSeconds(),
	//						0.f
	//					}
	//				);
	//			});
	//	});

	SceneManager::GetInstance().AddScene(__MOVE(Scene, testScene));

	core.Run();
}