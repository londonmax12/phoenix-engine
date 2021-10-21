#pragma once
#include <string>
#include "../vendor/glm/glm/glm.hpp"
#include "Phoenix/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace phx {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, const int& value) override;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, const int& value);

		void UploadUniformFloat(const std::string& name, const float& value);
		void UploadUniformVec2(const std::string& name, const glm::vec2& vec2);
		void UploadUniformVec3(const std::string& name, const glm::vec3& vec3);
		void UploadUniformVec4(const std::string& name, const glm::vec4& vec4);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);		
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

		uint32_t m_RendererID;
		std::string m_Name;
	};

}