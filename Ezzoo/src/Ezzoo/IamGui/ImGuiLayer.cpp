#include "ezpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"

#include "GLFW/glfw3.h"
#include "backends/imgui_impl_opengl3.h"

#include "Ezzoo/Application.h"
namespace Ezzoo {


	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
	{

	}
	ImGuiLayer::~ImGuiLayer()
	{

	}


	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;     // Enable Keyboard Controls
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;      // Enable Gamepad Controls

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;

		ImGui_ImplOpenGL3_Init("#version 410");
		
	}
	void ImGuiLayer::OnDetach()
	{

	}
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetApplication();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f) / (60.0f);
		m_Time = time;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMoveEvent>(EZZOO_BIND(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(EZZOO_BIND(ImGuiLayer::OnMouseMoveScrolledEvent));
		dispatcher.Dispatch<MousePressedEvent>(EZZOO_BIND(ImGuiLayer::OnMousePressedEvent));
		dispatcher.Dispatch<MouseReleasedEvent>(EZZOO_BIND(ImGuiLayer::OnMouseReleasedEvent));

		dispatcher.Dispatch<KeyPressedEvent>(EZZOO_BIND(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(EZZOO_BIND(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch <KeyTypedEvent>(EZZOO_BIND(ImGuiLayer::OnKeyTypedEvent));

		dispatcher.Dispatch<WindowCloseEvent>(EZZOO_BIND(ImGuiLayer::OnWindowClosedEvent));
		dispatcher.Dispatch<WindowResizedEvent>(EZZOO_BIND(ImGuiLayer::OnWindowResizedEvent));
		dispatcher.Dispatch<WindowMovedEvent>(EZZOO_BIND(ImGuiLayer::OnWindowMovedEvent));
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetXPos(), e.GetYPos());

		return false;
	}
	bool ImGuiLayer::OnMouseMoveScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYoffset();
		io.MouseWheelH += e.GetXoffset();

		return false;
	}
	bool ImGuiLayer::OnMousePressedEvent(MousePressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		
		return false;
	}
	bool ImGuiLayer::OnMouseReleasedEvent(MouseReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;


		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned int keyCode = (unsigned int)e.GetKeyCode();

		if (keyCode > 0 && keyCode < 0x10000)
		{
			io.AddInputCharacter(keyCode);
		}

		return false;
	}
	bool ImGuiLayer::OnWindowResizedEvent(WindowResizedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWindowWidth(), (float)e.GetWindowHeight());
		return false;
	}
	bool ImGuiLayer::OnWindowMovedEvent(WindowMovedEvent& e)
	{
		return false;
	}
	bool ImGuiLayer::OnWindowClosedEvent(WindowCloseEvent& e)
	{
		return false;
	}
}