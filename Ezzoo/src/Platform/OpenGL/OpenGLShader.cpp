#include "ezpch.h"

#include "OpenGLShader.h"

#include "Ezzoo/Core.h"
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

namespace Ezzoo {


	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string shaderStr =  ReadFile(filepath);
		auto shaders = PreProcess(shaderStr);

		Compile(shaders);

		//assets/Texture.glsl
		size_t lastSlash = filepath.find_last_of("/\\");
		EZZOO_CORE_ASSERT(lastSlash != std::string::npos, "No Slashes in File path, Wrong file path format!!!");

		size_t firstLetterIndex = lastSlash + 1;
		size_t dot = filepath.find_last_of(".");
		EZZOO_CORE_ASSERT(dot != std::string::npos, "No Dot in File path, Wrong file path format!!!");

		std::string name = filepath.substr(firstLetterIndex, (dot - firstLetterIndex));

		m_Name = name;

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaders;
		shaders[GL_VERTEX_SHADER] = vertexSrc;
		shaders[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(shaders);
			
	}
	OpenGLShader::~OpenGLShader()
	{
			glDeleteProgram(m_RendererID);
	}

	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		EZZOO_CORE_ASSERT(false, "Not Valid shader Type!!!");

		return 0;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string shaderStr;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			shaderStr.resize(in.tellg());

			in.seekg(0, std::ios::beg);
			in.read(&shaderStr[0], shaderStr.size());

			in.close();
		}
		else
		{
			EZZOO_CORE_ASSERT(false, "File is not available in this Path "+ filepath +"!!!");
		}

		return shaderStr;

	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& result)
	{
			std::unordered_map<GLenum, std::string> resultShaderMap;

			const char* typeToken = "#type";
			size_t typeTokenLength = strlen(typeToken);
			size_t pos = result.find(typeToken, 0);

			//#type vertex

			while (pos != std::string::npos)
			{
				size_t eol = result.find_first_of("\r\n", pos);
				EZZOO_CORE_ASSERT(eol != std::string::npos, "Syntax Error");

				size_t begin = pos + typeTokenLength + 1;
				std::string type = result.substr(begin, (eol - begin));
				EZZOO_CORE_ASSERT(type == "vertex" || type == "fragment", "Invalid Shader Type!!!");

				size_t nextLinePos = result.find_first_not_of("\r\n", eol);
				pos = result.find(typeToken, nextLinePos);

				resultShaderMap[ShaderTypeFromString(type)] = result.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? result.size() : nextLinePos));
			}
			
			return resultShaderMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
	{
			GLint program = glCreateProgram();
			EZZOO_CORE_ASSERT(shaders.size() <= 2, "We only support 2 shaders for now");
			std::array<GLenum, 2> shaderIDs;
			// GLenum *shaderIDs = (GLenum *)alloca(shaders.size() * sizeof(GLenum)); // alternative to allocate on stack
			int shaderIDIndex = 0;
			for (auto& kv : shaders)
			{

				GLenum type = kv.first;
				const std::string& source = kv.second;

			
				GLuint shader = glCreateShader(type);
				
				const GLchar* sourceCstr = source.c_str();
				glShaderSource(shader, 1, &sourceCstr, 0);
				
				glCompileShader(shader);

				GLint isCompiled = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				
					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

					
					glDeleteShader(shader);

					EZZOO_CORE_ERROR("{0}", infoLog.data());
					EZZOO_CORE_ASSERT(false, "Shader compilation faild");
					break;
				}

				glAttachShader(program, shader);
				shaderIDs[shaderIDIndex++] = shader;
			}

			// Link our program
			glLinkProgram(program);

			// Note the different functions here: glGetProgram* instead of glGetShader*.
			GLint isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(program);

				for (auto id : shaderIDs)
					// Don't leak shaders either.
					glDeleteShader(id);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				EZZOO_CORE_ERROR("{0}", infoLog.data());
				EZZOO_CORE_ASSERT(false, "Shader Link compilation faild");
				return;
			}

			// Always detach shaders after a successful link.
			for (auto id : shaderIDs)
				glDetachShader(program, id);

			m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	int OpenGLShader::GetLocation(const std::string& name)
	{
		int locationVal;

		if (m_CashedValues.find(name) != m_CashedValues.end())
				locationVal = m_CashedValues[name];
		else
		{
				locationVal = glGetUniformLocation(m_RendererID, name.c_str());
				m_CashedValues[name] = locationVal;
		}

		return locationVal;
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& values)
	{
		GLint locationVal = GetLocation(name);
		glUniformMatrix3fv(locationVal, 1, GL_FALSE, glm::value_ptr(values));

	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& values)
	{
		GLint locationVar = GetLocation(name);
		glUniformMatrix4fv(locationVar, 1, GL_FALSE, glm::value_ptr(values));
		
	}


	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint locationVar = GetLocation(name);
		glUniform1f(locationVar, value);

	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint locationVar = GetLocation(name);
		glUniform2f(locationVar, values.x, values.y);

	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint locationVar = GetLocation(name);
		glUniform3f(locationVar, values.x, values.y, values.z);

	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint locationVar = GetLocation(name);
		glUniform4f(locationVar, values.x, values.y, values.z, values.w);

	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint locationVar = GetLocation(name);
		glUniform1i(locationVar, value);
	}
}
