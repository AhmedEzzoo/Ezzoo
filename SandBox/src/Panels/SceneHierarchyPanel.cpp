#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include "Scene/Component.h"


namespace Ezzoo {


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}


	void SceneHierarchyPanel::OnImGuiRender()
	{

		ImGui::Begin("SceneHeirarchyPanel");

		 m_Context->m_Registry.each([&](auto entityID) {


				Entity ent { entityID, m_Context.get() };
				DrawEntityNode(ent);

			});


		 
		ImGui::End();

		//static bool open = true;
		//ImGui::ShowDemoWindow(&open);

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();
		if (ImGui::TreeNode((void*)(uint64_t)(uint32_t)entity, tag.TagName.c_str()))
		{
			ImGui::Text("%s", tag.TagName.c_str());
			ImGui::TreePop();
		}

	}

}