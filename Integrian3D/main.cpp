#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "MeshComponent/MeshComponent.h"

int main()
{
	using namespace Integrian3D;

	Core core{ 640,480 };

	Scene testScene{ "TestScene" };

	std::vector<Vertex> vertices =
	{
		 Vertex{ Vector3f{ 0.5f, -0.5f, 0.0f }, RGBColour{ 1.0f, 0.0f, 0.0f } },   // bottom right
		 Vertex{ Vector3f{ -0.5f, -0.5f, 0.0f }, RGBColour{ 0.0f, 1.0f, 0.0f} },   // bottom left
		 Vertex{ Vector3f{ 0.0f,  0.5f, 0.0f }, RGBColour{ 0.0f, 0.0f, 1.0f } }    // top
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2,   // first triangle
	};

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, vertices, indices);

	SceneManager::GetInstance().AddScene(std::move(testScene));

	core.Run();
}