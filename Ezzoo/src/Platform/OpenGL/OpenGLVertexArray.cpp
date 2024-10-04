#include "ezpch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Ezzoo {

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat2:		return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;

		}

		return 0;
	}



	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
		
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const 
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		
		const auto& layout = vertexBuffer->GetLayout();

#if 0		//uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertwxBufferIndex);
			glVertexAttribPointer(m_VertwxBufferIndex,
				element.GetComponentCount(),
				GL_FLOAT,
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStrid(),
				(const void*)element.Offset);
			m_VertwxBufferIndex++;
		}
#endif	
		for (const auto& element : layout)
		{

			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertwxBufferIndex);
				glVertexAttribPointer(m_VertwxBufferIndex,
					element.GetComponentCount(),
					GL_FLOAT,
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStrid(),
					(const void*)element.Offset);
				m_VertwxBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertwxBufferIndex);
				glVertexAttribPointer(m_VertwxBufferIndex,
					element.GetComponentCount(),
					GL_FLOAT,
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStrid(),
					(const void*)element.Offset);
				m_VertwxBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertwxBufferIndex);
					glVertexAttribPointer(m_VertwxBufferIndex,
						count,
						GL_FLOAT,
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStrid(),
						reinterpret_cast<const void*>(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertwxBufferIndex, 1);
					m_VertwxBufferIndex++;
				}
			
				break;
			}


			}

		
		}
		

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}