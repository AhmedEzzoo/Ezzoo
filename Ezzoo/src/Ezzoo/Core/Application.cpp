#include "ezpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Ezzoo/Core/Core.h"

#include "Ezzoo/Core/TimeStep.h"

#include "Scripting/ScriptEngine/ScriptEngine.h"


namespace Ezzoo {

	Application* Application::s_Instance = nullptr;


	Application::Application(const ApplicationSpecification& specs)
		: m_Specification(specs)
		
	{

		EZZOO_CORE_ASSERT(!s_Instance, "Application Already running");
		//if (s_Instance) return;
		s_Instance = this;

		m_AppWindow = std::unique_ptr<Window>(Window::Create({ m_Specification.Name }));

		if (!m_Specification.path.empty())
			std::filesystem::current_path(m_Specification.path);

		m_AppWindow->SetEventCallBack(EZZOO_BIND(Application::OnEvent));
		
		Renderer::Init();
		ScriptEngine::Init();


		m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);

	}
	Application::~Application()
	{
		//delete m_ImGuiLayer;
	}

	void Application::Run()
	{

		while (m_Running)
		{
			
			if (!m_Minimized)
			{	
				float time = (float)glfwGetTime();
				TimeStep ts = time - m_LastMeasuredTime;
				m_LastMeasuredTime = time;


				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);

			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_AppWindow->OnUpdate();
		}
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
		dispatch.Dispatch<WindowResizedEvent>(EZZOO_BIND(Application::OnWindowResizedEvent));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);

			if (event.m_Handeled) break;
		}

		//EZZOO_CLIENT_TRACE("{0}", event);
	}


	Window& Application::GetWindow()
	{
		return *m_AppWindow;
	}

	Application& Application::GetApplication()
	{
		return *s_Instance;
	}

	void Application::Close()
	{
		m_Running = false;
	}


	bool Application::OnWindowResizedEvent(WindowResizedEvent& e)
	{
		if (e.GetWindowWidth() == 0 || e.GetWindowHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWindowWidth(), e.GetWindowHeight());

		return false;
	}



	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}


}