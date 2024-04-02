#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "RendererAPI.h"

namespace Ezzoo {

	class RendererCommand
	{
	public :

		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		static void ClearColor()
		{
			s_RendererAPI->ClearColor();
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private :
		static RendererAPI* s_RendererAPI;
	};
}