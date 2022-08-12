#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"

int main()
{
	using namespace Integrian3D;

	Core core{ 640,480 };

	Scene testScene{ "TestScene" };

	std::vector<Vertex> vertices =
	{
		// positions          // colors           // texture coords
		Vertex{ glm::vec3{ 0.5f,  0.5f, 0.0f },   RGBColour{ 1.0f, 0.0f, 0.0f },   glm::vec2{ 1.0f, 1.0f } },   // top right
		Vertex{ glm::vec3{ 0.5f, -0.5f, 0.0f },   RGBColour{ 0.0f, 1.0f, 0.0f },   glm::vec2{ 1.0f, 0.0f } },   // bottom right
		Vertex{ glm::vec3{ -0.5f, -0.5f, 0.0f },   RGBColour{ 0.0f, 0.0f, 1.0f },   glm::vec2{ 0.0f, 0.0f } },   // bottom left
		Vertex{ glm::vec3{ -0.5f,  0.5f, 0.0f },   RGBColour{ 1.0f, 1.0f, 0.0f },   glm::vec2{ 0.0f, 1.0f } }    // top left 
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 3,   // first triangle
		1, 2, 3,   // first triangle
	};

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
	TransformComponent& transform{ testScene.GetComponent<TransformComponent>(entity) };
	transform.Rotate(glm::vec3{ 0.f, 0.f, 1.f }, MathUtils::ToRadians(90.f));

	SceneManager::GetInstance().AddScene(std::move(testScene));

	core.Run();
}