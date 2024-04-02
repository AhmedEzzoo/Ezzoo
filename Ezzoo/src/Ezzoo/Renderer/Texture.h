#pragma once

#include "Ezzoo/Core.h"

namespace Ezzoo {

	class Texture
	{
	public :
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;


		virtual void Bind(uint8_t slot = 0) const = 0;

	private :

	};


	class Texture2D : public Texture
	{ 
		public :
			virtual ~Texture2D() = default;

			static Ref<Texture2D> CreateTexture2D(const std::string& filepath);
	};
}