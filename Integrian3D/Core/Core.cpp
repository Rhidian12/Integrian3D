#include "Core.h"

#include "../DebugUtility/DebugUtility.h"
#include "../Timer/Timer.h"
#include "../InputManager/InputManager.h"
#include "../SceneManager/SceneManager.h"
#include "../Components/MeshComponent/MeshComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

#include <string_view>

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

	Core::Core(const int windowWidth, const int windowHeight)
		: Window{ windowWidth, windowHeight }
	{
		InputManager::CreateInputManager(&Window);
	}

	void Core::Shutdown()
	{
		Logger::Cleanup();
	}

	Core& Core::GetInstance()
	{
		__ASSERT(Instance != nullptr, "Core::GetInstance() > Ensure Core::CreateCore is called before Core::GetInstance");

		return *Instance.get();
	}

	Core& Core::CreateCore(const int windowWidth, const int windowHeight)
	{
		__ASSERT(Instance == nullptr, "Core::CreateCore() > This function cannot be called more than once");

		Instance = std::unique_ptr<Core>(new Core{ windowWidth, windowHeight });

		srand(static_cast<unsigned int>(time(nullptr)));

		Math::SetSeed(SEED);

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(LogGLError, nullptr);

		LOG(CoreLog, LogErrorLevel::Log, "Finished initialisation of engine");

		return *Instance.get();
	}

	void Core::Run()
	{
		using namespace Time;

		g_IsRunning = true;

		Timer& timer{ Timer::GetInstance() };
		InputManager& inputManager{ InputManager::GetInstance() };
		SceneManager& sceneManager{ SceneManager::GetInstance() };

		for (Scene* const pScene : sceneManager.GetAllScenes())
		{
			pScene->Start();
		}

		double lag{};
		const double timePerFrame{ timer.GetFixedElapsedTime<TimeLength::MilliSeconds>() };

		while (g_IsRunning)
		{
			lag += timer.GetElapsedTime<TimeLength::MilliSeconds>();

			timer.Update();

			inputManager.ProcessInput();

			Scene* const pActiveScene{ sceneManager.GetActiveScene() };

			pActiveScene->Update();

			while (lag >= timePerFrame)
			{
				lag -= timePerFrame;
			}

			pActiveScene->Render();

			/* Swap buffers */
			Window.Update();

			// LogMessage("FPS: " + std::to_string(Timer::GetInstance().GetFPS()), false);
		}

		Shutdown();
	}
}