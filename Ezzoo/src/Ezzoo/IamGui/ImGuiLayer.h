#pragma once

#include "Layer.h"
#include "Ezzoo/Events/MouseEvent.h"
#include "Ezzoo/Events/KeyEvent.h"
#include "Ezzoo/Events/WindowEvent.h"
namespace Ezzoo {

	class ImGuiLayer :public Layer
	{

	public :

		ImGuiLayer();
		virtual ~ImGuiLayer();


		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private :
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseMoveScrolledEvent(MouseScrolledEvent& e);
		bool OnMousePressedEvent(MousePressedEvent& e);
		bool OnMouseReleasedEvent(MouseReleasedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowClosedEvent(WindowCloseEvent& e);
		bool OnWindowResizedEvent(WindowResizedEvent& e);
		bool OnWindowMovedEvent(WindowMovedEvent& e);

	private :
		float m_Time{ 0.0f };


	};
}