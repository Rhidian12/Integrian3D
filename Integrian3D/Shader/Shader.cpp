#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../FileReader/FileReader.h"
#include "../DebugUtility/DebugUtility.h"

namespace Integrian3D
{
	Shader::Shader(const std::string& filePath)
		: ShaderID{}
	{
		const FileReader reader{ filePath };

		/* Generate VertexShader ID */
		ShaderID = glCreateShader(GL_VERTEX_SHADER);

		/* Attach the VertexShader code to the ID and compile it */
		const char* pShaderSource{ reader.GetFileContents().c_str() };
		glShaderSource(ShaderID, 1, &pShaderSource, nullptr);
		glCompileShader(ShaderID);

		/* Check if the compilation succeeded */
		int success{};
		char infoLog[512]{};
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(ShaderID, 512, nullptr, infoLog);
			Debug::LogError(std::string("Shader compilation failed: ") + infoLog, false);
		}
	}
}