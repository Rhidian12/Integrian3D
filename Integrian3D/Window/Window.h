#pragma once

#include "EngineConstants.h"

DEFINE_LOG_CATEGORY(WindowLog, Integrian3D::LogVerbosity::Verbose);

namespace Integrian3D
{
	namespace Detail
	{
		class Window final
		{
		public:
			Window(const int width, const int height);
			~Window();

			Window(const Window&) noexcept = delete;
			Window(Window&&) noexcept = delete;
			Window& operator=(const Window&) noexcept = delete;
			Window& operator=(Window&&) noexcept = delete;

			void Update();

			__NODISCARD int GetWidth() const { return Width; }
			__NODISCARD int GetHeight() const { return Height; }

		private:
			int Width;
			int Height;
		};
	}
}