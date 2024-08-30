#pragma once

#include "Ezzoo.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Ezzoo {

	class Sandbox2D : public Layer
	{

	public:
		Sandbox2D();

		virtual ~Sandbox2D() = default;


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;



	private:
		Ref<Shader> m_QuadShader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Texture2D> m_Texture;

		Ref<FrameBuffer> m_FrameBuffer;

		OrthgraphicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;

		Entity m_GreenSquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;

		SceneHierarchyPanel m_ScenePanel;

		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewPortSize = { 0.0f, 0.0f };
	};
}