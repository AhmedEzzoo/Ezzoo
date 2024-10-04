#pragma once

#include "Ezzoo/Core/Core.h"

#include <filesystem>

namespace Ezzoo {

	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint8_t slot = 0) const = 0;

		virtual bool operator== (const Texture& other) const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

	private :

	};


	class Texture2D : public Texture
	{ 
		public :
			virtual ~Texture2D() = default;

			static Ref<Texture2D> CreateTexture2D(uint32_t width, uint32_t height);
			static Ref<Texture2D> CreateTexture2D(const std::string& filepath);
			//static Ref<Texture2D> CreateTexture2D(const std::filesystem::path& filepath);
	};
}