#pragma once

#ifdef PHX_PLATFORM_WINDOWS

extern phx::Application* phx::CreateApplication();

int main(int argc, char** argv)
{
	auto app = phx::CreateApplication();
	app->Run();
	delete app;
}
#endif

