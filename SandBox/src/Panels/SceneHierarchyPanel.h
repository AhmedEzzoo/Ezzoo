#pragma once

#include "Ezzoo/Core/Core.h"

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

	private :
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);


	private :

		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};

}