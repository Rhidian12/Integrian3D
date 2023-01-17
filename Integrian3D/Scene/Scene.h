#pragma once

#include "../EngineConstants.h"
#include "../Array/Array.h"
#include "../GameObject/GameObject.h"

#include <functional> /* std::function */
#include <utility> /* std::forward */
#include <string> /* std::string */
#include <string_view> /* std::string_view */
#include <vector> /* std::vector */

namespace Integrian3D
{
	class Scene
	{
	public:
		explicit Scene(const std::string& sceneName);

		/* -------------- Begin of Scene Functionality -------------- */

		__NODISCARD std::string_view GetSceneName() const { return SceneName; }

		void AddSceneInitialisation(const std::function<void(Scene&)>& fn) { InitializeCallback = fn; }

		void AddOnSceneEnter(const std::function<void(Scene&)>& fn) { OnSceneEnterCallback = fn; }

		void AddOnSceneLeave(const std::function<void(Scene&)>& fn) { OnSceneLeaveCallback = fn; }

		void AddGameObject(GameObject* const pGameobject);

		/* -------------- End of Scene Functionality -------------- */


		/* -------------- Start of Internal Functionality -------------- */

		void Start();

		void OnSceneEnter() { OnSceneEnterCallback(*this); }

		void OnSceneLeave() { OnSceneLeaveCallback(*this); }

		void Update();

		/* -------------- End of Internal Functionality -------------- */

	private:
		std::function<void(Scene&)> InitializeCallback;
		std::function<void(Scene&)> OnSceneEnterCallback;
		std::function<void(Scene&)> OnSceneLeaveCallback;
		std::string SceneName;
		TArray<GameObject*> m_GameObjects;
	};
}