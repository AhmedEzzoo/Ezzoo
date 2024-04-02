#pragma once

#include "Ezzoo/Renderer/RendererAPI.h"

namespace Ezzoo {

	class OpenGLRenderer : public RendererAPI
	{
	public :
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void ClearColor() override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	private :

	};
}