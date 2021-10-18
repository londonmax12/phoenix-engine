#pragma once
#include <string>
#include "../vendor/glm/glm/glm.hpp"
#include "Phoenix/Renderer/Shader.h"

namespace phx {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, const int& value);

		void UploadUniformFloat(const std::string& name, const float& value);
		void UploadUniformVec2(const std::string& name, const glm::vec2& vec2);
		void UploadUniformVec3(const std::string& name, const glm::vec3& vec3);
		void UploadUniformVec4(const std::string& name, const glm::vec4& vec4);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};

}