#pragma once


#include "Ezzoo/Renderer/Texture.h"

#include "glad/glad.h"

namespace Ezzoo {

	class  OpenGLTexture2D : public Texture2D
	{
	public :

		OpenGLTexture2D(uint32_t width, uint32_t height);

		OpenGLTexture2D(const std::string& filepath );

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint8_t slot = 0) const override;


		virtual uint32_t GetRendererID() const override { return m_RendererID; };
		virtual const std::string& GetPath() const override { return m_Path;  };




		virtual bool operator== (const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}


	private :
		uint32_t m_RendererID;

		std::string m_Path;

		uint32_t m_Width{0};
		uint32_t m_Height{0};

		GLenum m_InternalFormat, m_DataFormat;
	};
}