#pragma once

#include <Filesystem>

//#include "Ezzoo/Core/Core.h"
#include "Ezzoo/Renderer/Texture.h"

namespace Ezzoo {

	class ContentBrowser
	{
	public :

		ContentBrowser();

		void OnImGuiRender();

	private :
		
		std::filesystem::path m_ContentDirectory;
		Ref<Texture2D> m_FolderTexture;
		Ref<Texture2D> m_FileTexture;
		std::string m_RelativesString = std::string();
	};

}