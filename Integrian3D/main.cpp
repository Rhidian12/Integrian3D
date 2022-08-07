#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "MeshComponent/MeshComponent.h"

int main()
{
	using namespace Integrian3D;

	Core core{ 640,480 };

	Scene testScene{ "TestScene" };

	std::vector<float> vertices =
	{
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, vertices, indices);

	SceneManager::GetInstance().AddScene(std::move(testScene));

	core.Run();
}