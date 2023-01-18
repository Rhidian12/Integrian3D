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

		/* -------------- Begin of Scene Functionality -------------- */

		void AddGameObject(GameObject* const pGameobject);

		__NODISCARD std::string_view GetSceneName() const { return m_SceneName; }

		__NODISCARD class CameraComponent* const GetActiveCamera() const { return m_pActiveCamera; }

		/* -------------- End of Scene Functionality -------------- */


		/* -------------- Start of Internal Functionality -------------- */

		void Start();

		void Update();

		void Render() const;

		/* -------------- End of Internal Functionality -------------- */


		/* -------------- Start of Inheritance Functionality -------------- */

		virtual void OnSceneEnter() {}

		virtual void OnSceneLeave() {}

		/* -------------- End of Inheritance Functionality -------------- */

	private:
		std::string m_SceneName;
		TArray<GameObject*> m_GameObjects;
		class CameraComponent* m_pActiveCamera;
	};
}