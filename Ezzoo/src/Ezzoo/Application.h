#pragma once

#include "Window.h"
#include "Ezzoo/Events/WindowEvent.h"
#include "Ezzoo/Events/KeyEvent.h"

#include "LayerStack.h"
#include "IamGui/ImGuiLayer.h"

#include "Ezzoo/Renderer/Renderer.h"

namespace Ezzoo {

	class Application
	{

	public:

		Application();
		virtual ~Application();
		void Run();

		
		void OnEvent(Event &event);
		
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		static Application& GetApplication();

		Window& GetWindow();

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool Application::OnWindowResizedEvent(WindowResizedEvent& e);

	private:
		std::unique_ptr<Window> m_AppWindow;
		ImGuiLayer* m_ImGuiLayer = nullptr;
		LayerStack m_LayerStack;
		bool m_Running{ true };
		bool m_Minimized = false;

		static Application* s_Instance;

		float m_LastMeasuredTime = 0.0f;

	};

	Application* CreatApplication();

}



