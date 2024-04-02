#pragma once

#include "Ezzoo/Renderer/Shader.h"

#include "glm/glm.hpp"



typedef unsigned int GLenum;
namespace Ezzoo {

	class OpenGLShader : public Shader
	{
	public :

		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~OpenGLShader();

		virtual void Bind() const override ;
		virtual void Unbind() const override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& values);
		void UploadUniformMat4(const std::string& name, const glm::mat4& values);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformInt(const std::string& name, int value);
		
		const std::string& GetName() const override { return m_Name; }
	
	private :
	
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& result);
		GLenum ShaderTypeFromString(const std::string& type);
		void Compile(const std::unordered_map<GLenum, std::string>& shaders);

		int GetLocation(const std::string& name);


	private :
		uint32_t m_RendererID = 0;
		std::string m_Name;
		std::unordered_map<std::string, int> m_CashedValues;


	
	};
}