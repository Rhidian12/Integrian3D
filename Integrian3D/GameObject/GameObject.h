#pragma once

#include "../EngineConstants.h"
#include "../Object/Object.h"
#include "../Component/Component.h"
#include "../Array/Array.h"

#include <string> /* std::string */

namespace Integrian3D
{
	class GameObject : public Object
	{
	public:
		virtual ~GameObject();

		void AddComponent(Component* const pComp);

		void Update();
		void Render() const;

	private:
		GameObject();
		explicit GameObject(const std::string& name);

		friend GameObject* Instantiate(const std::string&);
		friend GameObject* Instantiate(const std::string&, class Scene*);

		TArray<Component*> m_Components;
	};

#pragma region Helper_Functions
	
	__NODISCARD GameObject* Instantiate(const std::string& name = "");
	__NODISCARD GameObject* Instantiate(const std::string& name, class Scene* pScene);

#pragma endregion
}