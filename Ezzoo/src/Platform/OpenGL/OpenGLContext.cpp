#include "ezpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
namespace Ezzoo {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		
	}
	OpenGLContext::~OpenGLContext()
	{
		EZZOO_CORE_INFO("OpenGLContext Destructor Called");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		//auto vendor = (void*)(glGetString(GL_VENDOR));

		//EZZOO_CORE_INFO("OpenGL Renderer is : {0}", glGetString(GL_VENDOR));


	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}