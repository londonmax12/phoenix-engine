#include "phxpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace phx {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}