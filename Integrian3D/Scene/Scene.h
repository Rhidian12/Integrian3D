#pragma once

#include "../EngineConstants.h"

#include "../Array/Array.h"
#include "../TPair/TPair.h"
#include "../ECS/Registry/Registry.h"

#include <string>
#include <string_view>

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		// Does not accept member functions, only free functions
		void AddInitialisationCallback(const size_t prio, void(*fn)(Scene&));

		std::string_view GetSceneName() const { return m_SceneName; }

#pragma region ECS_Functionality

		__NODISCARD Entity CreateEntity() { return m_Registry.CreateEntity(); }

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

#pragma endregion

		void Start();

		void Update();

		void Render() const;

	private:
		// Scene Info
		std::string m_SceneName;
		TArray<TPair<size_t, void(*)(Scene&)>> m_InitCallbacks; // Does not accept member functions

		// ECS Info
		Registry m_Registry;
	};
}