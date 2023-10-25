#pragma once

#include "../../EngineConstants.h"

#include "../../Array/Array.h"
#include "../../Component/Component.h"

#include <string>
#include <string_view>

namespace Integrian3D
{
	// Can be inherited from
	class TagComponent : public Component
	{
	public:
		virtual ~TagComponent() = default;

		void ForEach(void(*fn)(const std::string&)) const;

		virtual void AddTag(const std::string& tag);

		void SetTag(std::string_view tag, const std::string& newTag);
		void SetTagByIndex(const int32 i, const std::string& tag);

		void RemoveTag(std::string_view tag);
		void RemoveTagByIndex(const int32 i);

		__NODISCARD bool HasTag(std::string_view tag) const;

		__NODISCARD std::string_view GetTagByIndex(const int32 i) const;

	protected:
		TArray<std::string> m_Tags;
	};
}