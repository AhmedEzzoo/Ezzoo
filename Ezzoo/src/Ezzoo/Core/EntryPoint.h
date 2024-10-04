#pragma once
#include "Log.h"

#include "Application.h"
#include "Instrumentor.h"

extern Ezzoo::Application* Ezzoo::CreatApplication(Ezzoo::ApplicationSpecification& specs);

#ifdef EZZOO_WINDOWS_64

int main(int argc, char** argv)
{
	
	EZZOO_BEGINE_SESSION();

	Ezzoo::Log::Init();
	auto app = Ezzoo::CreatApplication(Ezzoo::ApplicationSpecification());

	EZZOO_CORE_WARNING("Intialization of Engine");

	EZZOO_CLIENT_INFO("Hello!");

	app->Run();

	delete app;
	
	EZZOO_END_SESSION();
}
#endif