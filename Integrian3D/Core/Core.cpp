#include "Core.h"

#include "Components/MeshComponent/MeshComponent.h"
#include "Components/TransformComponent/TransformComponent.h"
#include "DebugUtility.h"
#include "IO/FileContentCache/FileContentCache.h"
#include "InputManager/InputManager.h"
#include "SceneManager/SceneManager.h"
#include "TextureManager/TextureManager.h"
#include "Thread/ThreadManager.h"
#include "Thread/ThreadUtils.h"
#include "Timer/Timer.h"
#include "Window/Window.h"

#include <string_view>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Integrian3D
{
	namespace
	{
		static void LogGLError(GLenum Source, GLenum Type, GLuint ID, GLenum Severity, GLsizei /*Length*/, const GLchar* message, const void* /*UserParam*/)
		{
			if (!(Type == GL_DEBUG_TYPE_ERROR ||
				Type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ||
				Type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
				Type == GL_DEBUG_TYPE_PERFORMANCE))
			{
				return;
			}

			std::string_view SourceStr{}, TypeStr{}, SeverityStr{};

			switch (Source)
			{
				case GL_DEBUG_SOURCE_API:
					SourceStr = "API";
					break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
					SourceStr = "WINDOW SYSTEM";
					break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER:
					SourceStr = "SHADER COMPILER";
					break;
				case GL_DEBUG_SOURCE_THIRD_PARTY:
					SourceStr = "THIRD PARTY";
					break;
				case GL_DEBUG_SOURCE_APPLICATION:
					SourceStr = "APPLICATION";
					break;
				case GL_DEBUG_SOURCE_OTHER:
					SourceStr = "UNKNOWN";
					break;
				default:
					SourceStr = "UNKNOWN";
					break;
			}

			switch (Type)
			{
				case GL_DEBUG_TYPE_ERROR:
					TypeStr = "ERROR";
					break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					TypeStr = "DEPRECATED BEHAVIOR";
					break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					TypeStr = "UDEFINED BEHAVIOR";
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					TypeStr = "PORTABILITY";
					break;
				case GL_DEBUG_TYPE_PERFORMANCE:
					TypeStr = "PERFORMANCE";
					break;
				case GL_DEBUG_TYPE_OTHER:
					TypeStr = "OTHER";
					break;
				case GL_DEBUG_TYPE_MARKER:
					TypeStr = "MARKER";
					break;
				default:
					TypeStr = "UNKNOWN";
					break;
			}

			switch (Severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:
					SeverityStr = "HIGH";
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					SeverityStr = "MEDIUM";
					break;
				case GL_DEBUG_SEVERITY_LOW:
					SeverityStr = "LOW";
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					SeverityStr = "NOTIFICATION";
					break;
				default:
					SeverityStr = "UNKNOWN";
					break;
			}

			LOG(ShaderLog, LogErrorLevel::Error, "{}: {} of {} severity, raised from {}: {}", ID, TypeStr, SeverityStr, SourceStr, message);
		}
	}

	Core::Core() {}

	void Core::Shutdown()
	{
		SceneManager::GetInstance().ClearAllScenes();

		Threading::ThreadManager::GetInstance().StopAllThreads();
		TextureManager::Cleanup();

		IO::FileContentCache::StartCleanup();

		__DELETE(GWindow);

		LOG(CoreLog, LogErrorLevel::Log, "Shutdown of Engine is complete");

		Logger::StartCleanup();
	}

	Core& Core::GetInstance()
	{
		__ASSERT(GWindow != nullptr, "Core::GetInstance() > Ensure Core::CreateCore is called before Core::GetInstance");

		static Core Instance{};

		return Instance;
	}

	Core& Core::CreateCore(const int windowWidth, const int windowHeight)
	{
		Logger::Initialize();

		__ASSERT(GWindow == nullptr, "Core::CreateCore() > This function cannot be called more than once");

		GWindow = new Window{ windowWidth, windowHeight };

		srand(static_cast<unsigned int>(time(nullptr)));

		Math::SetSeed(SEED);

		GMainThreadID = GetThreadID();

		#if _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(LogGLError, nullptr);
		#endif

		LOG(CoreLog, LogErrorLevel::Log, "Finished initialisation of engine");

		return GetInstance();
	}

	void Core::Run()
	{
		using namespace Time;

		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		SceneManager& SceneManager{ SceneManager::GetInstance() };

		SceneManager.InitializeAllScenes();

		double lag{};
		const double timePerFrame{ timer.GetFixedElapsedTime<TimeLength::MilliSeconds>() };

		while (g_IsRunning)
		{
			lag += timer.GetElapsedTime<TimeLength::MilliSeconds>();

			timer.Update();

			inputManager.ProcessInput();

			Scene* const pActiveScene{ SceneManager.GetActiveScene() };

			pActiveScene->Update();

			while (lag >= timePerFrame)
			{
				lag -= timePerFrame;
			}

			pActiveScene->Render();

			// Swap buffers
			GWindow->Update();

			// LogMessage("FPS: " + std::to_string(Timer::GetInstance().GetFPS()), false);
		}

		Shutdown();
	}
}