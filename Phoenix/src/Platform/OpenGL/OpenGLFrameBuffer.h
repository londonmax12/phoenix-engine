#pragma once

#include "Phoenix/Renderer/Framebuffer.h"

namespace phx {
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Resize();

		void Bind() override;
		void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}
