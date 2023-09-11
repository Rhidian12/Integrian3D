#pragma once

#include "../EngineConstants.h"

#include "../TPair/TPair.h"
#include "../ECS/Registry/Registry.h"
#include "../ECS/View/View.h"

#include <functional>
#include <string>
#include <string_view>

namespace Integrian3D
{
	class FreeCameraComponent;

	class Scene final
	{
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		// Does not accept member functions, only free functions
		void AddInitialisationCallback(const size_t prio, const std::function<void(Scene&)>& Function);

		// Does not accept member functions, only free functions
		void AddUpdateCallback(const size_t prio, const std::function<void(Scene&)>& Function);

		// Does not accept member functions, only free functions
		void AddRenderCallback(const size_t prio, const std::function<void(Scene&)>& Function);

		__NODISCARD std::string_view GetSceneName() const { return m_SceneName; }

		__NODISCARD __INLINE Entity GetActiveCameraEntity() const { return m_ActiveCameraEntity; }

#pragma region ECS_Functionality

		__NODISCARD Entity CreateEntity();

		// [TODO]: Should be a const function
		template<typename ... Ts>
		__NODISCARD View<Ts...> CreateView() { return m_Registry.CreateView<Ts...>(); }

		template<typename T>
		T& AddComponent(const Entity entity) { return m_Registry.AddComponent<T>(entity); }
		template<typename T, typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args) { return m_Registry.AddComponent<T, Ts...>(entity, std::forward<Ts>(args)...); }

		template<typename T>
		void RemoveComponent(const Entity entity) { m_Registry.RemoveComponent<T>(entity); }

		template<typename T>
		__NODISCARD T& GetComponent(const Entity entity) { return m_Registry.GetComponent<T>(entity); }
		template<typename T>
		__NODISCARD const T& GetComponent(const Entity entity) const { return m_Registry.GetComponent<T>(entity); }

		__NODISCARD bool HasEntity(const Entity entity) const { return m_Registry.HasEntity(entity); }

		bool ReleaseEntity(const Entity entity) { return m_Registry.ReleaseEntity(entity); }
		
		void ClearEntities() { return m_Registry.Clear(); }

		template<typename T>
		__NODISCARD bool HasComponent(const Entity entity) const { return m_Registry.HasComponent<T>(entity); }

		template<typename T>
		__NODISCARD Entity FindEntity(const T& comp) { return m_Registry.FindEntity(comp); }

		__NODISCARD __INLINE size_t GetNrOfEntities() const { return m_Registry.GetAmountOfEntities(); }

		template<typename ... Ts>
		__NODISCARD bool CanViewBeCreated() const { return m_Registry.CanViewBeCreated<Ts...>(); }

#pragma endregion

		void Start();

		void Update();

		void Render();

	private:
		// Scene Info
		std::string m_SceneName;
		std::vector<TPair<size_t, std::function<void(Scene&)>>> m_InitCallbacks; // Does not accept member functions
		std::vector<TPair<size_t, std::function<void(Scene&)>>> m_UpdateCallbacks; // Does not accept member functions
		// [TODO]: SHOULD BE CONST SCENE&
		std::vector<TPair<size_t, std::function<void(Scene&)>>> m_RenderCallbacks; // Does not accept member functions
		Entity m_ActiveCameraEntity;

		// ECS Info
		Registry m_Registry;
	};
}