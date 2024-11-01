#pragma once

//#include "Ezzoo/Core/Core.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Ezzoo {


	class SceneHierarchyPanel
	{

	public :
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);


		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

	private :
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
		template <typename T>
		void DisplayAddComponentEntry(const std::string& name)
		{
			if (!m_SelectedEntity.HasComponent<T>())
			{
				if (ImGui::MenuItem(name.c_str()))
				{
					m_SelectedEntity.AddComponent<T>();
					ImGui::CloseCurrentPopup();
				}
			}
		}

	private :

		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};

}