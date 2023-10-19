#pragma once

#include "../EngineConstants.h"
#include "../Array/Array.h"

#include <functional> /* std::function */

namespace Integrian3D
{
	template<typename ... Ts>
	class Delegate final
	{
	public:
		using Callback = std::function<void(const Ts...)>;

		Delegate() = default;

		Delegate(const Delegate& Other)
			: Callbacks{ Other.Callbacks }
		{}

		Delegate(Delegate&& Other)
			: Callbacks{ __MOVE(Other.Callbacks) }
		{}

		Delegate& operator=(const Delegate& Other)
		{
			Callbacks = Other.Callbacks;

			return *this;
		}

		Delegate& operator=(Delegate&& Other)
		{
			Callbacks = __MOVE(Other.Callbacks);

			return *this;
		}

		void Bind(const Callback& fn)
		{
			Callbacks.Add(fn);
		}

		void Bind(Callback&& fn)
		{
			Callbacks.Add(std::move(fn));
		}

		void Unbind(const Callback& fn)
		{
			Callbacks.Erase(fn);
		}

		template<typename ... Us>
		void Invoke(Us&&... vals) const
		{
			for (const Callback& fn : Callbacks)
			{
				fn(__FORWARD(vals)...);
			}
		}

	private:
		TArray<Callback> Callbacks;
	};
}