#pragma once


#include "Ezzoo/Renderer/Texture.h"

namespace Ezzoo {

	class  OpenGLTexture2D : public Texture2D
	{
	public :

		OpenGLTexture2D(const std::string& filepath );

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint8_t slot = 0) const override;


	private :
		uint32_t m_RendererID;

		std::string m_Path;

		uint32_t m_Width{0};
		uint32_t m_Height{0};
	};
}