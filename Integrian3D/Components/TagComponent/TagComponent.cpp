#include "TagComponent.h"

namespace Integrian3D
{
	void TagComponent::ForEach(void(*fn)(const std::string&)) const
	{
		for (const std::string& tag : m_Tags)
		{
			fn(tag);
		}
	}

	void TagComponent::AddTag(const std::string& tag)
	{
		if (!m_Tags.Contains(tag))
		{
			m_Tags.Add(tag);
		}
	}

	void TagComponent::SetTag(std::string_view tag, const std::string& newTag)
	{
		auto it{ m_Tags.Find([tag](const std::string& _tag)->bool
			{
				return tag == _tag;
			}) };

		if (it != m_Tags.end())
		{
			*it = newTag;
		}
	}

	void TagComponent::SetTagByIndex(const int32 i, const std::string& tag)
	{
		__ASSERT(i < m_Tags.Size(), "TagComponent::SetTagByIndex() > Index %i is out of range", i);

		m_Tags[i] = tag;
	}

	void TagComponent::RemoveTag(std::string_view tag)
	{
		m_Tags.Erase([tag](const std::string& _tag)->bool
			{
				return tag == _tag;
			});
	}

	void TagComponent::RemoveTagByIndex(const int32 i)
	{
		__ASSERT(i < m_Tags.Size(), "TagComponent::RemoveTagByIndex() > Index %i is out of range", i);

		m_Tags.EraseByIndex(i);
	}

	bool TagComponent::HasTag(std::string_view tag) const
	{
		return m_Tags.Contains([tag](const std::string& _tag)->bool
			{
				return tag == _tag;
			});
	}

	std::string_view TagComponent::GetTagByIndex(const int32 i) const
	{
		__ASSERT(i < m_Tags.Size(), "TagComponent::GetTagByIndex() > Index %i is out of range", i);

		return m_Tags[i];
	}
}