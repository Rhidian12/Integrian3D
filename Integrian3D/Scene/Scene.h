#pragma once

#include "../EngineConstants.h"
#include "../ECS/Registry/Registry.h"
#include "../Components/CameraComponent/CameraComponent.h"

#include <functional> /* std::function */
#include <utility> /* std::forward */
#include <string> /* std::string */
#include <string_view> /* std::string_view */
#include <vector> /* std::vector */

namespace Integrian3D
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& sceneName);

		/* -------------- Begin of ECS Functionality -------------- */
		__NODISCARD Entity CreateEntity();

		bool ReleaseEntity(const Entity entity) { return Registry.ReleaseEntity(entity); }

		template<typename T>
		T& AddComponent(const Entity entity) { return Registry.AddComponent<T>(entity); }

		template<typename T, typename ... Ts>
		T& AddComponent(const Entity entity, Ts&& ... args) { return Registry.AddComponent<T, Ts...>(entity, std::forward<Ts>(args)...); }

		template<typename T>
		void RemoveComponent(const Entity entity) { Registry.RemoveComponent<T>(entity); }

		template<typename T>
		__NODISCARD T& GetComponent(const Entity entity) { return Registry.GetComponent<T>(entity); }

		template<typename T>
		__NODISCARD const T& GetComponent(const Entity entity) const { return Registry.GetComponent<T>(entity); }

		__NODISCARD size_t GetAmountOfEntities() const { return static_cast<size_t>(Registry.GetAmountOfEntities()); }

		__NODISCARD EntitySignature GetEntitySignature(const Entity entity) const { return Registry.GetEntitySignature(entity); }

		template<typename ... Ts>
		__NODISCARD View<Ts...> CreateView() { return Registry.CreateView<Ts...>(); }
		/* -------------- End of ECS Functionality -------------- */


		/* -------------- Begin of Scene Functionality -------------- */
		__NODISCARD std::string_view GetSceneName() const { return SceneName; }

		void AddSceneInitialisation(const std::function<void(Scene&)>& fn) { InitializeCallback = fn; }

		void AddOnSceneEnter(const std::function<void(Scene&)>& fn) { OnSceneEnterCallback = fn; }

		void AddOnSceneLeave(const std::function<void(Scene&)>& fn) { OnSceneLeaveCallback = fn; }

		void AddUpdateCallback(const std::function<void(Scene&)>& fn) { UpdateCallbacks.push_back(fn); }

		__NODISCARD CameraComponent& GetCamera() { return GetComponent<CameraComponent>(CameraEntity); }

		__NODISCARD Entity GetCameraEntity() const { return CameraEntity; }

		__NODISCARD const std::vector<std::function<void(Scene&)>>& GetUpdateCallbacks() const { return UpdateCallbacks; }
		/* -------------- End of Scene Functionality -------------- */


		/* -------------- Start of Internal Functionality -------------- */
		void Start();

		void OnSceneEnter() { OnSceneEnterCallback(*this); }

		void OnSceneLeave() { OnSceneLeaveCallback(*this); }
		/* -------------- End of Internal Functionality -------------- */

	private:
		std::function<void(Scene&)> InitializeCallback;
		std::function<void(Scene&)> OnSceneEnterCallback;
		std::function<void(Scene&)> OnSceneLeaveCallback;
		std::vector<std::function<void(Scene&)>> UpdateCallbacks;
		Registry Registry;
		std::string SceneName;
		Entity CameraEntity;
	};
}