#pragma once

extern Ezzoo::Application* CreatApplication();

int main(int argc, char** argv)
{
	auto app = CreatApplication();

	app->Run();

	delete app;
}