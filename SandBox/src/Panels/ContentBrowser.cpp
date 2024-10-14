
#include "ContentBrowser.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Ezzoo {
	
	 extern const std::filesystem::path s_AssetsDirectory = "assets";

	ContentBrowser::ContentBrowser()
		: m_ContentDirectory(s_AssetsDirectory)
	{
		m_FolderTexture = Texture2D::CreateTexture2D("assets/Pictures/DirectoryIcon.png");
		m_FileTexture = Texture2D::CreateTexture2D("assets/Pictures/FileIcon.png");

	}


	void ContentBrowser::OnImGuiRender()
	{

		//static bool open = true;
		ImGui::Begin("Content Browser");// , & open, ImGuiWindowFlags_NoScrollbar);
		auto windowSize = ImGui::GetWindowSize();

		if (m_ContentDirectory != s_AssetsDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_ContentDirectory = m_ContentDirectory.parent_path();
			}
		}

		static float windowPadding = 16.0f;
		static  float iconSize = 60.0f;

		float cellSize = windowPadding + iconSize;

		float columnWidth = ImGui::GetContentRegionAvail().x;

		uint8_t noOfColumns = (uint8_t)(columnWidth / cellSize);

		if (noOfColumns < 1) noOfColumns = 1;

		ImGui::Columns(noOfColumns, 0, false);

		for (auto& entry : std::filesystem::directory_iterator{ m_ContentDirectory })
		{
			//float size;
			const auto& path = entry.path();
			//auto relativePath = std::filesystem::relative(path, m_ContentDirectory);
			m_RelativesString = path.filename().string();


			 ImGui::PushID(m_RelativesString.c_str());
			
			 Ref<Texture2D> icon = entry.is_directory() ? m_FolderTexture : m_FileTexture;
			 ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 1.0f });
			 ImGui::ImageButton("DragDrop", (void*)icon->GetRendererID(), ImVec2(iconSize, iconSize), ImVec2{0, 1}, ImVec2{1, 0});


			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("Path", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				

				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemClicked() && ImGui::IsItemHovered())
			{
				if (entry.is_directory())
					m_ContentDirectory /= path.filename();
			}

			ImGui::TextWrapped(m_RelativesString.c_str());

			ImGui::PopStyleColor();

			ImGui::NextColumn();



			ImGui::PopID();
			
		}
		ImGui::Columns(1);

		ImGui::SliderFloat("Icons", &iconSize, 16.0f, 512.0f);
		ImGui::SliderFloat("Padding", &windowPadding, 0.0f, 32.0f);

		ImGui::End();

	}

}