#pragma once

#include "../vendor/glm/glm/glm.hpp"

namespace phx {
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

		const glm::vec4& GetClearColor() const { return m_ClearColor; }
		void SetClearColor(glm::vec4 color) { m_ClearColor = color; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::vec4 m_ClearColor = { 0.12f, 0.12f, 0.12f, 1.f };
	};
}