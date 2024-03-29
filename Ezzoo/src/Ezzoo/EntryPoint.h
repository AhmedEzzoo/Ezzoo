#pragma once
#include "Log.h"

extern Ezzoo::Application* Ezzoo::CreatApplication();

#ifdef EZZOO_WINDOWS_64

int main(int argc, char** argv)
{
	Ezzoo::Log::Init();
	auto app = Ezzoo::CreatApplication();

	EZZOO_CORE_WARNING("Intialization of Engine");

	EZZOO_CLIENT_INFO("Hello!");

	app->Run();

	delete app;
}
#endif