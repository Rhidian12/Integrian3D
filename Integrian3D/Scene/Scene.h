#pragma once

#include "../ECS/Registry/Registry.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& sceneName);

		void AddSceneInitialisation(const std::function<void(const Scene&)>& fn) { InitializeCallback = fn; }
		void AddOnSceneEnter(const std::function<void(const Scene&)>& fn) { OnSceneEnterCallback = fn; }
		void AddOnSceneLeave(const std::function<void(const Scene&)>& fn) { OnSceneLeaveCallback = fn; }

		/* -------------- Begin of ECS Functionality -------------- */
		Entity CreateEntity();

		bool ReleaseEntity(const Entity entity) { return Registry.ReleaseEntity(entity); }

		template<typename T>
		T& AddComponent(const Entity entity) { return Registry.AddComponent<T>(entity); }

		template<typename T, typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args) { return Registry.AddComponent<T, Ts...>(entity, args...); }

		template<typename T>
		void RemoveComponent(const Entity entity) { Registry.RemoveComponent<T>(entity); }

		template<typename T>
		T& GetComponent(const Entity entity) { return Registry.GetComponent<T>(entity); }

		template<typename T>
		const T& GetComponent(const Entity entity) const { return Registry.GetComponent<T>(entity); }

		size_t GetAmountOfEntities() const { return static_cast<size_t>(Registry.GetAmountOfEntities()); }

		const EntitySignature GetEntitySignature(const Entity entity) const { return Registry.GetEntitySignature(entity); }
		/* -------------- End of ECS Functionality -------------- */

		const std::string& GetSceneName() const { return SceneName; }


		/* -------------- Start of Internal Functionality -------------- */
		void Start();

		void OnSceneEnter() { OnSceneEnterCallback(*this); }

		void OnSceneLeave() { OnSceneLeaveCallback(*this); }
		/* -------------- End of Internal Functionality -------------- */

	private:
		std::function<void(Scene&)> InitializeCallback;
		std::function<void(Scene&)> OnSceneEnterCallback;
		std::function<void(Scene&)> OnSceneLeaveCallback;
		Registry Registry;
		std::string SceneName;
	};
}