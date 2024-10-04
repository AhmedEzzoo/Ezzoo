#pragma once

#include "Ezzoo.h"


#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowser.h"


namespace Ezzoo {

	class Sandbox2D : public Layer
	{
		enum class SceneStatus
		{
			Play = 0,
			Edit = 1,
			Simulate = 2
		};



	public:
		Sandbox2D();

		virtual ~Sandbox2D() = default;


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;


	private :

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene(const std::filesystem::path& path);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnMOusePressedEvent(MousePressedEvent& event);

		void OnOverLayUpdate();

		void OnScenePlay();
		void OnSceneEdit();
		void OnSceneSimulate();


		void OnDuplicate();


	private:
		Ref<Shader> m_QuadShader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_PlayButtonTexture;
		Ref<Texture2D> m_StopButtonTexture;
		Ref<Texture2D> m_SimulateButtonTexture;

		Ref<FrameBuffer> m_FrameBuffer;

		//OrthgraphicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Entity m_GreenSquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;
		Entity m_HoveredEntity;

		SceneHierarchyPanel m_ScenePanel;
		ContentBrowser m_ContentBrowser;

		EditorCamera m_EditorCamera;

		bool m_PrimaryCamera = true;
		bool m_ViewPortHovered = false;
		bool m_ViewPortFocused = false;

		bool m_ShowPhysicsColliders = false;

		glm::vec2 m_ViewPortSize = { 0.0f, 0.0f };

		int m_GuizmoType = -1;

		glm::vec2 m_ViewPortBounds[2];

		SceneStatus m_SceneStatus = SceneStatus::Edit;

		std::filesystem::path m_ScenePath;

	};
}
