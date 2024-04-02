#include "ezpch.h"

#include "Texture.h"

#include "Ezzoo/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Ezzoo {


	Ref<Texture2D> Texture2D::CreateTexture2D(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:  EZZOO_CORE_ASSERT(false, "None is not a Renderer!!!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(filepath);
		}

		EZZOO_CORE_ASSERT(false, "No Supported Renderer!!!!!");
		return nullptr;
	}
}