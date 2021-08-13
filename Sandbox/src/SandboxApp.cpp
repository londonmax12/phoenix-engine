#include <Phoenix.h>

class Sandbox : public phx::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

phx::Application* phx::CreateApplication()
{
	return new Sandbox();
}