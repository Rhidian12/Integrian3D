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
		virtual ~Scene();

#pragma region Scene_Functionality

		void AddGameObject(GameObject* const pGameobject);

		void RemoveGameObject(GameObject* const pGameObject);

		__NODISCARD std::string_view GetSceneName() const { return m_SceneName; }

		__NODISCARD class CameraComponent* const GetActiveCamera() const { return m_pActiveCamera; }

		__NODISCARD const TArray<GameObject*>& GetGameObjects() const { return m_GameObjects; }

#pragma endregion

#pragma region Internal_Functionality

		void Start();

		void Update();

		void Render() const;

#pragma endregion

#pragma region Inheritable_Functionality

		virtual void OnSceneEnter() {}

		virtual void OnSceneLeave() {}

#pragma endregion

	private:
		std::string m_SceneName;
		TArray<GameObject*> m_GameObjects;
		class CameraComponent* m_pActiveCamera;
	};
}