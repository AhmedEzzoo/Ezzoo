#include "ezpch.h"
#include "Application.h"
//#include "GLFW/glfw3.h"
#include "Ezzoo/Core.h"

#include <glad/glad.h>
namespace Ezzoo {

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		if (s_Instance) return;
		s_Instance = this;
		m_AppWindow = std::unique_ptr<Window>(Window::Create());
		m_AppWindow->SetEventCallBack(EZZOO_BIND(Application::OnEvent));

		unsigned int id;

		glGenVertexArrays(1, &id);

	}
	Application::~Application()
	{

	}

	Window&	Application::GetWindow()
	{
		return *m_AppWindow;
	}

	Application& Application::GetApplication()
	{
		return *s_Instance;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event &event)
	{
		EventDispatcher dispatch(event);

		dispatch.Dispatch<WindowCloseEvent>(EZZOO_BIND(Application::OnWindowCloseEvent));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);

			if (event.m_Handeled) break;
		}

		//EZZOO_CLIENT_TRACE("{0}", event);
	}
	void Application::Run()
	{

		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			m_AppWindow->OnUpdate();
		}
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}