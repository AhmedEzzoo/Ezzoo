#pragma once

#include "Window.h"

#include "GLFW/glfw3.h"

namespace Ezzoo {

	class WindowsWindow : public Window
	{
	public :

		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		
		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;

		virtual void SetEventCallBack(const EventCallBack& callBack) override { m_Data.CallBack = callBack; };

		virtual void SetVSync(bool enable) override;
		virtual bool IsVSync() const override ;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

	private :
		void Init(const WindowProps& props);
		void ShutDown();


	private :

		GLFWwindow* m_Window;
		
		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned Height;
			bool VSync;

			EventCallBack CallBack;
		};

		WindowData m_Data;

	};

}