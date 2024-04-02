#include "ezpch.h"

#include "Renderer.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Ezzoo {

	Renderer::SceneData* Renderer::s_Data = new SceneData();

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(OrthoGraphicCamera& camera)
	{
		s_Data->ViewProjection =  camera.GetViewProjection();

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		vertexArray->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_Data->ViewProjection);
		RendererCommand::DrawIndexed(vertexArray);
	}
}