#include "Core/Core.h"
#include "Scene/Scene.h"
#include "SceneManager/SceneManager.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "TextureManager/TextureManager.h"
#include "Components/TransformComponent/TransformComponent.h"

int main()
{
	using namespace Integrian3D;

	Core& core{ Core::CreateCore(640,480) };

	Scene testScene{ "TestScene" };

	std::vector<Vertex> vertices =
	{
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },

		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },

		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f },  glm::vec2{ 1.0f, 1.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f },  glm::vec2{ 0.0f, 1.0f } }
	};

	std::vector<uint32_t> indices{};

	for (size_t i{}; i < vertices.size(); ++i)
	{
		indices.push_back(i);
	}

	TextureManager::GetInstance().AddTexture("__Wall", "Resources/wall.jpg");

	Entity entity = testScene.CreateEntity();
	testScene.AddComponent<MeshComponent>(entity, vertices, indices, TextureManager::GetInstance().GetTexture("__Wall"));
	TransformComponent& transform{ testScene.GetComponent<TransformComponent>(entity) };
	transform.Rotate(glm::vec3{ 1.f, 0.f, 0.f }, MathUtils::ToRadians(-55.f));

	SceneManager::GetInstance().AddScene(std::move(testScene));

	core.Run();
}