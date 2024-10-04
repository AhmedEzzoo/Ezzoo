#pragma once

#include "Window.h"
#include "Ezzoo/Events/WindowEvent.h"
#include "Ezzoo/Events/KeyEvent.h"

#include "LayerStack.h"
#include "Ezzoo/IamGui/ImGuiLayer.h"

#include "Ezzoo/Renderer/Renderer.h"

#include <filesystem>

namespace Ezzoo {


	struct ApplicationSpecification
	{
		std::string Name;
		std::filesystem::path path;

		ApplicationSpecification() = default;
	};


	class Application
	{

	public:

		Application(const ApplicationSpecification& specs);
		virtual ~Application();
		void Run();

		
		void OnEvent(Event &event);
		
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);

		static Application& GetApplication();

		void Close();

		Window& GetWindow();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

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
		ApplicationSpecification m_Specification;
	};

	Application* CreatApplication(ApplicationSpecification& specs);

}



