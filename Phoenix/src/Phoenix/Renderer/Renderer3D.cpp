#include "phxpch.h"
#include "Renderer3D.h"

#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/UniformBuffer.h"
#include "Phoenix/Renderer/RenderCommand.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace phx
{
	void Renderer3D::Init()
	{
		m_MeshShader = Shader::Create("MeshShader", "assets/shaders/Renderer3D_Mesh.vert", "assets/shaders/Renderer3D_Mesh.frag");
	}
	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjection();
	}
	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);;
	}
	void Renderer3D::EndScene()
	{
	}
	void Renderer3D::SubmitMesh(Mesh& mesh, glm::mat4 transform, int entityID)
	{
		std::dynamic_pointer_cast<OpenGLShader>(m_MeshShader)->UploadUniformMat4("u_ViewProjection", m_SceneData.ViewProjectionMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(m_MeshShader)->UploadUniformMat4("u_Transform", model);

		m_MeshShader->Bind();
		mesh.m_VertexArray->Bind();
		RenderCommand::DrawIndexed(mesh.m_VertexArray);
	}
}