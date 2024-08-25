#pragma once

#include "Ezzoo/Core/Layer.h"
#include "Ezzoo/Events/MouseEvent.h"
#include "Ezzoo/Events/KeyEvent.h"
#include "Ezzoo/Events/WindowEvent.h"
namespace Ezzoo {

	class ImGuiLayer :public Layer
	{

	public :

		ImGuiLayer();
		virtual ~ImGuiLayer();


		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;


		void Begin();
		void End();

	private :
		float m_Time{ 0.0f };


	};
}