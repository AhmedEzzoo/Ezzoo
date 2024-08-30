#pragma once

#include "Ezzoo/Core/Core.h"

#include "Scene/Scene.h"

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


	private :

		Ref<Scene> m_Context;

	};

}