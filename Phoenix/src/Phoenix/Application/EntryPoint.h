#pragma once


#ifdef PHX_PLATFORM_WINDOWS

extern phx::Application* phx::CreateApplication();

int main(int argc, char** argv)
{
#ifdef PHX_DEBUG_MODE
	phx::Log::Init();
	PHX_CORE_INFO("Phoenix Intialized Logging System");	
#endif

	auto app = phx::CreateApplication();
	app->Run();
	delete app;
	
}
#endif

