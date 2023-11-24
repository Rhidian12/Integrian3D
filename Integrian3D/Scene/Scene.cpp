#include "Scene.h"

#include "Core/Core.h"
#include "InputManager/InputManager.h"
#include "IO/Ini/IniFile.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "TPair/TPair.full.h"

#include "Components/FreeCameraComponent/FreeCameraComponent.h"
#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TagComponent/TagComponent.h"
#include "Components/TransformComponent/TransformComponent.h"

#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"

#include "Window/Window.h"

#include <gtc/matrix_transform.hpp>

namespace Integrian3D
{
	Scene::Scene(const std::string& sceneName)
		: m_SceneName{ sceneName }
		, m_InitCallbacks{}
		, m_UpdateCallbacks{}
		, m_RenderCallbacks{}
		, m_Registry{}
	{
		// Render meshes
		AddRenderCallback(UpdateOrder::CoreUpdate, [](Scene& Scene)->void
			{
				if (Scene.GetNrOfEntities() == 0u)
				{
					return;
				}

				Renderer& Renderer{ Renderer::GetInstance() };

				// [CRINGE]: Shitty way of handling the camera
				IASSERT((Scene.CanViewBeCreated<TransformComponent, FreeCameraComponent>()));

				{
					const View CameraView{ Scene.CreateView<TransformComponent, FreeCameraComponent>() };
					IASSERT(CameraView.GetNrOfEntities() == 1);

					CameraView.ForEach([&Renderer](const auto& Transform, const auto& Camera)->void
						{
							Renderer.StartRenderLoop(Camera, Transform);
						});
				}

				if (Scene.CanViewBeCreated<TransformComponent, PointLight>())
				{
					Scene.CreateView<TransformComponent, PointLight>().ForEach
					(
						[&Renderer](const TransformComponent& Transform, const PointLight& Light)->void
						{
							Renderer.CollectPointLight(Transform, Light);
						}
					);
				}

				if (Scene.CanViewBeCreated<TransformComponent, DirectionalLight>())
				{
					Scene.CreateView<TransformComponent, DirectionalLight>().ForEach
					(
						[&Renderer](const TransformComponent& Transform, const DirectionalLight& Light)->void
						{
							Renderer.CollectDirectionalLight(Transform, Light);
						}
					);
				}

				if (Scene.CanViewBeCreated<TransformComponent, MeshComponent>())
				{
					const View view{ Scene.CreateView<TransformComponent, MeshComponent>() };

					view.ForEach([&Renderer](const auto& transform, auto& mesh)->void
						{
							Renderer.Render(mesh, transform);
						});
				}
			});

		// Transform update
		AddUpdateCallback(UpdateOrder::CoreUpdate, [](Scene& scene)->void
			{
				if (scene.GetNrOfEntities() > 0u && scene.CanViewBeCreated<TransformComponent>())
				{
					const View view{ scene.CreateView<TransformComponent>() };

					view.ForEach([](TransformComponent& transform)->void
						{
							transform.RecalculateTransform();
						});
				}
			});

		// Camera update
		AddUpdateCallback(UpdateOrder::CoreUpdate, [](Scene& scene)->void
			{
				if (scene.GetNrOfEntities() > 0u && scene.CanViewBeCreated<FreeCameraComponent, TransformComponent>())
				{
					const View view{ scene.CreateView<FreeCameraComponent, TransformComponent>() };

					view.ForEach([](FreeCameraComponent& camera, TransformComponent& transform)->void
						{
							camera.UpdateView(transform);

							if (!InputManager::GetInstance().GetIsMouseButtonPressed(MouseInput::RMB)) return;

							camera.UpdateTranslation(transform);
							camera.UpdateRotation(transform);
						});
				}
			});
	}

	Scene::~Scene()
	{
		m_Registry.Clear();
	}

	void Scene::AddInitialisationCallback(const UpdateOrder prio, const std::function<void(Scene&)>& Function)
	{
		m_InitCallbacks.emplace_back(MakePair(prio, Function));
	}

	void Scene::AddUpdateCallback(const UpdateOrder prio, const std::function<void(Scene&)>& Function)
	{
		m_UpdateCallbacks.emplace_back(MakePair(prio, Function));
	}

	void Scene::AddRenderCallback(const UpdateOrder prio, const std::function<void(Scene&)>& Function)
	{
		m_RenderCallbacks.emplace_back(MakePair(prio, Function));
	}

	Entity Scene::CreateEntity()
	{
		const Entity entity{ m_Registry.CreateEntity() };

		m_Registry.AddComponent<TransformComponent>(entity);

		return entity;
	}

	void Scene::Start()
	{
		m_ActiveCameraEntity = InvalidEntityID;

		std::sort(m_InitCallbacks.begin(), m_InitCallbacks.end(), [](const auto& a, const auto& b)->bool
			{
				return a.Key() < b.Key();
			});

		for (const auto& [prio, callback] : m_InitCallbacks)
		{
			callback(*this);
		}

		if (m_Registry.CanViewBeCreated<FreeCameraComponent, TagComponent>())
		{
			const View view{ m_Registry.CreateView<FreeCameraComponent, TagComponent>() };

			view.ForEach([this](const auto& camera, const auto& tag)->void
				{
					if (tag.HasTag("MainCamera"))
					{
						this->m_ActiveCameraEntity = FindEntity(camera);
					}
				});
		}

		if (m_ActiveCameraEntity == InvalidEntityID)
		{
			m_ActiveCameraEntity = CreateEntity();

			AddComponent<FreeCameraComponent>
				(
					m_ActiveCameraEntity,
					0.1f,
					10000.f,
					Math::ToRadians(45.f),
					static_cast<float>(GWindow->GetWidth() / GWindow->GetHeight()),
					100.f
					);

			IniFile EngineIni{ "Config/Engine.ini" };

			Math::Vec3D CameraPosition{};
			if (!EngineIni.GetVector3D("Scene", "CameraStartPosition", CameraPosition))
			{
				CameraPosition = Math::Vec3D{ 0.0, 10.0, 10.0 };
			}

			m_Registry.GetComponent<TransformComponent>(m_ActiveCameraEntity).Translate(CameraPosition);
		}

		std::sort(m_UpdateCallbacks.begin(), m_UpdateCallbacks.end(), [](const auto& a, const auto& b)->bool
			{
				return a.Key() < b.Key();
			});
	}

	void Scene::Update()
	{
		for (const auto& [prio, callback] : m_UpdateCallbacks)
		{
			callback(*this);
		}
	}

	void Scene::Render()
	{
		for (const auto& [prio, callback] : m_RenderCallbacks)
		{
			callback(*this);
		}
	}
}