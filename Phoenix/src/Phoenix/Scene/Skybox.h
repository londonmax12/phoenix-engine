#pragma once

#include "Phoenix/Renderer/Texture.h"
#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/Renderer2D.h"
#include "Phoenix/Application/Base.h"
#include "Phoenix/Application/Application.h"

#include <string>
#include <vector> 

namespace phx {
	class SkyBox {
	public:
		SkyBox(std::string filepath)
			: m_Path(filepath)
		{
			m_SingleImage = true;
			m_Textures[0] = Texture2D::Create(filepath);
			m_Shader = Shader::Create("Renderer_Skybox", "assets/shaders/Renderer_Skybox.vert", "assets/shaders/Renderer_Skybox.frag");
		}
		SkyBox(std::vector<std::string> files)
		{
			if (files.size() != 6)
			{
				PHX_CORE_WARN("Skybox creation failed: Amount of images does not equal 6");
				return;
			}
			else
			{
				m_SingleImage = false;
				for (int i = 0; i < 6; i++)
				{
					m_Textures[i] = Texture2D::Create(files[i]);
				}
				m_Shader = Shader::Create("Renderer_Skybox", "assets/shaders/Renderer_Skybox.vert", "assets/shaders/Renderer_Skybox.frag");
			}
		}
		void Render() {
			m_Shader->Bind();

			Renderer2D::DrawQuadFilled(glm::vec3(-m_Size, m_Size, -m_Size), glm::vec2(m_Size, m_Size), m_Textures[0]);
		}
		Ref<Texture2D> m_Textures[6];
	private:
		bool m_SingleImage = true;
		
		float m_Size = 1000.0f;
		std::string m_Path = std::string();
		Ref<Shader> m_Shader;
	};
}