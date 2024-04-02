#pragma once

#include "Ezzoo/Renderer/Buffers.h"
namespace Ezzoo {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public :
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();


		virtual void Bind()  const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;

	private :
		uint32_t m_RendererID{0};
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indecies, uint32_t count);

		virtual ~OpenGLIndexBuffer();


		virtual uint32_t GetCount() const override { return m_Count; }

		virtual void Bind()  const override;
		virtual void Unbind() const override;


	private:
		uint32_t m_RendererID{0};
		uint32_t m_Count;
	};
}
