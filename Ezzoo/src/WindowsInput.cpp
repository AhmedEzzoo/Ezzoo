#include "ezpch.h"
#include "Input.h"
#include "Ezzoo/Application.h"
#include "GLFW/glfw3.h"

namespace Ezzoo {


	bool Input::IsKeyPressed(int keyCode)
	{
		auto status = glfwGetKey(static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow()), keyCode);
		return status;
	}
	bool Input::IsMousePressed(int mouseCode)
	{
		auto status =glfwGetMouseButton(static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow()), mouseCode);
		return status;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPos, yPos;
		
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}


}