

#include "Sandbox2D.h"
#include <cmath>

#include "Scene/SceneSerialize.h"
#include "Ezzoo/Renderer/Renderer.h"
#include "Ezzoo/Renderer/Renderer2D.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "ImGuizmo/ImGuizmo.h"



namespace Ezzoo {

	extern const std::filesystem::path s_AssetsDirectory;

	Sandbox2D::Sandbox2D()
		: Layer("Sandbox2D")//, m_CameraController(1600.0f / 900.0f, true)
	{
	}


	void Sandbox2D::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_ScenePanel.SetContext(m_ActiveScene);

		m_ScenePath = std::filesystem::path();
	}

	void Sandbox2D::OpenScene()
	{
		std::string filepath = SceneSerialize::OpenFile("Ezzoo Scene(*.ezzoo)\0*.ezzoo\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void Sandbox2D::OpenScene(const std::filesystem::path& path)
	{
		

		if (m_SceneStatus != SceneStatus::Edit)
			OnSceneEdit();

		if (path.extension().string() != ".ezzoo")
			return;

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerialize serialize(newScene);
		if (serialize.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_EditorScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_ScenePanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_ScenePath = path;
		}
		
	}

	void Sandbox2D::SaveSceneAs()
	{
		std::string filepath = SceneSerialize::SaveFile("Ezzoo Scene(*.ezzoo)\0 * .ezzoo\0");
		if (!filepath.empty()) SaveScene(filepath);
		//else return;
	}

	void Sandbox2D::SaveScene(const std::filesystem::path& path)
	{
		if (!path.string().empty())
		{
			SceneSerialize serialize(m_ActiveScene);
			serialize.Serialize(path.string());
			m_ScenePath = path;
		}
		else
		{
			SaveSceneAs();
		}
		
	}

	void Sandbox2D::OnScenePlay()
	{

		m_SceneStatus = SceneStatus::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRunTimeStart();

		m_ScenePanel.SetContext(m_ActiveScene);

	}
	void Sandbox2D::OnSceneEdit()
	{
		m_SceneStatus = SceneStatus::Edit;

		m_ActiveScene->OnRunTimeStop();
		//m_ActiveScene->OnSimulationStop();
		m_ActiveScene = m_EditorScene;

		m_ScenePanel.SetContext(m_ActiveScene);
	}

	void Sandbox2D::OnSceneSimulate()
	{
		m_SceneStatus = SceneStatus::Simulate;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_ScenePanel.SetContext(m_ActiveScene);

	}

	void Sandbox2D::OnAttach()
	{
		m_Texture = Texture2D::CreateTexture2D("assets/Pictures/pic_1.png");
		m_PlayButtonTexture = Texture2D::CreateTexture2D("assets/Pictures/PlayButton.png");
		m_SimulateButtonTexture = Texture2D::CreateTexture2D("assets/Pictures/PlayButton.png");
		m_StopButtonTexture = Texture2D::CreateTexture2D("assets/Pictures/StopButton.png");


		FrameBufferSpecification specs;
		specs.Attachments = { FrameBufferTextureFormate::RGBA8, FrameBufferTextureFormate::RED_INTEGER, FrameBufferTextureFormate::Depth };
		specs.Width = 1280;
		specs.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(specs);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	#if 0
		m_GreenSquareEntity = m_ActiveScene->CreateEntity("Green Square");
		m_GreenSquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.8f, 0.3f, 1.0f });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip Space Camera");
		auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
		cc.PrimaryCamera = false;

		class CameraController : public ScriptableEntity
		{
		public:

			virtual ~CameraController() = default;

			virtual void OnCreate() override 
			{

			}

			virtual void OnDestroy() override 
			{

			}


			virtual void OnUpdate(TimeStep ts) override
			{
			}


		private:

		};

		m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

	#endif
		m_ScenePanel.SetContext(m_ActiveScene);

		
	}

	void Sandbox2D::OnDetach()
	{

	}

	void Sandbox2D::OnUpdate(TimeStep ts)
	{

		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f &&
			(spec.Width != (uint32_t)m_ViewPortSize.x ||
				spec.Height != (uint32_t)m_ViewPortSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			//m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}
		

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		//EZZOO_FUNCTION_SCOPE("Sandbox2D::OnUpdate");

		//m_CameraController.OnUpdate(ts);

		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::ClearColor();

		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneStatus)
		{

		case SceneStatus::Edit:
		{
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneStatus::Play:
		{
			m_ActiveScene->OnUpdateRunTime(ts);
			break;
		}

		case SceneStatus::Simulate:
		{
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateSimulate(ts, m_EditorCamera);
			break;
		}
		}

		
		auto [mx, my] = ImGui::GetMousePos();

		mx -= m_ViewPortBounds[0].x;
		my -= m_ViewPortBounds[0].y;

		glm::vec2 viewPortsize = m_ViewPortBounds[1] - m_ViewPortBounds[0];

		my = viewPortsize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewPortsize.x && mouseY < (int)viewPortsize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
			//EZZOO_CLIENT_WARNING("{0}", pixelData);
			//if (pixelData != -1) m_ScenePanel.SetSelectedEntity(m_HoveredEntity);
		}


		OnOverLayUpdate();

		m_FrameBuffer->UnBind();
	}


	void Sandbox2D::OnImGuiRender()
	{

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static bool p_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Shift+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				
				if (ImGui::MenuItem("Save...", "Ctrl+S"))
				{
					SaveScene(m_ScenePath);
				}

				if (ImGui::MenuItem("SaveAs...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}


				if (ImGui::MenuItem("Exit", NULL))
					Application::GetApplication().Close();

				
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_ScenePanel.OnImGuiRender();
		m_ContentBrowser.OnImGuiRender();

		

		ImGui::Begin("Stats");

		//std::string name;

		//if (m_HoveredEntity)
			//name = m_HoveredEntity.GetComponent<TagComponent>().TagName;
		ImGui::Text("Quad Indices Number: %d", Renderer2D::GetStats().GetIndeciesCount());
		ImGui::Text("Quad Vertex Number: %d", Renderer2D::GetStats().GetVertexCount());
		ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quad Numbers: %d", Renderer2D::GetStats().QuadCount);
		//ImGui::Text("Entity Name: %s", m_GreenSquareEntity.GetComponent<TagComponent>().TagName.c_str());

		//auto& color = m_GreenSquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//ImGui::ColorEdit4("Square Color", glm::value_ptr(color));




		//ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().GetTransform()[3]), 0.1f, 0.0f, 0.0f, "%.2f");

		//if (ImGui::Checkbox("Primary Camera", &m_PrimaryCamera))
		//{
		//	m_CameraEntity.GetComponent<CameraComponent>().PrimaryCamera = m_PrimaryCamera;
		//	m_SecondCameraEntity.GetComponent<CameraComponent>().PrimaryCamera = !m_PrimaryCamera;
		//}


		ImGui::End();

		ImGui::Begin("Settings");

		ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);

		ImGui::End();


		ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);


		auto viewPortMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewPortMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewPortOffset = ImGui::GetWindowPos();
		m_ViewPortBounds[0] = { viewPortMinRegion.x + viewPortOffset.x, viewPortMinRegion.y + viewPortOffset.y };
		m_ViewPortBounds[1] = { viewPortMaxRegion.x + viewPortOffset.x, viewPortMaxRegion.y + viewPortOffset.y };


		m_ViewPortHovered = ImGui::IsWindowHovered();
		m_ViewPortFocused = ImGui::IsWindowFocused();
		Application::GetApplication().GetImGuiLayer()->SetBlockEvents(!m_ViewPortHovered && !m_ViewPortFocused);


		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };
		/*if (m_ViewPortSize != *((glm::vec2*)&viewPortPanelSize) && viewPortPanelSize.x > 0 && viewPortPanelSize.y > 0)
		{
			m_FrameBuffer->Resize((uint32_t)viewPortPanelSize.x, (uint32_t)viewPortPanelSize.y);
			
		}*/
		uint32_t id = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Path"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				
				OpenScene(path);
				
			}
			ImGui::EndDragDropTarget();
		}

		

		//ImGuizmo
		Entity selectedEntity = m_ScenePanel.GetSelectedEntity();
		if (selectedEntity  && m_GuizmoType != -1 && m_SceneStatus == SceneStatus::Edit)
		{
				//m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetGizmoSizeClipSpace(0.10f);


				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				//Run time Camera
			   /* auto cameraEntity = m_ActiveScene->GetCameraPrimaryEntity();
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& cameraProj = camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/


				//Editor Camera
				const glm::mat4& cameraProj = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
					ImGuizmo::OPERATION(m_GuizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform));


				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);
					glm::vec3 deltaRotation = rotation - tc.Rotation;

					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
		}



		ImGui::End();
		
		ImGui::Begin("##Button", nullptr);



		float pos = (ImGui::GetWindowWidth() * 0.5f) - (20.0f * 0.5f);
		ImGui::SetCursorPos(ImVec2(pos - 20.0f, 1.0f));

		Ref<Texture2D> btnTexPlayAndEdit = (m_SceneStatus == SceneStatus::Edit || m_SceneStatus == SceneStatus::Simulate) ? m_PlayButtonTexture : m_StopButtonTexture;
		 
		uint32_t texIDPlayAndEdit = btnTexPlayAndEdit->GetRendererID();
		if (ImGui::ImageButton("PlayBtn",(void*)texIDPlayAndEdit, ImVec2{20.0f, 20.0f}))
		{
			if (m_SceneStatus == SceneStatus::Edit || m_SceneStatus == SceneStatus::Simulate)
			{
				OnScenePlay();
			}
			else if (m_SceneStatus == SceneStatus::Play )
			{
				OnSceneEdit();
			}
		}


		ImGui::SetCursorPos(ImVec2(pos + 20.0f, 1.0f));
		Ref<Texture2D> btnTexSimulateAndEdit = m_SceneStatus == SceneStatus::Edit || m_SceneStatus == SceneStatus::Play ? m_SimulateButtonTexture : m_StopButtonTexture;

		uint32_t texIDSimulateAndEdit = btnTexSimulateAndEdit->GetRendererID();
		if (ImGui::ImageButton("SimulateBtn",(void*)texIDSimulateAndEdit, ImVec2{20.0f, 20.0f}))
		{
			if (m_SceneStatus == SceneStatus::Edit || m_SceneStatus == SceneStatus::Play  )
			{
				OnSceneSimulate();
			}
			else if (m_SceneStatus == SceneStatus::Simulate)
			{
				OnSceneEdit();
			}
		}



		ImGui::End();

		ImGui::End();


	}

	bool Sandbox2D::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() > 0)
			return false;


		int keyCode = event.GetKeyCode();

		bool ctrl = Input::IsKeyPressed(EZZOO_KEY_LEFT_CONTROL) || Input::IsKeyPressed(EZZOO_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(EZZOO_KEY_LEFT_SHIFT) || Input::IsKeyPressed(EZZOO_KEY_RIGHT_SHIFT);



		switch (keyCode)
		{
		case EZZOO_KEY_N :

			if (shift)
				NewScene();
			break;
		case EZZOO_KEY_O :
			if (ctrl)
				OpenScene();
			break;
		case EZZOO_KEY_S :
		{

			if (ctrl && shift)
				SaveSceneAs();

			else if (ctrl)
				SaveScene(m_ScenePath);
			break;
		}

		case EZZOO_KEY_D:
			if (ctrl)
				OnDuplicate();
			break;

		
		case EZZOO_KEY_Q:
			if (!ImGuizmo::IsUsing())
				m_GuizmoType = -1;
			break;

		case EZZOO_KEY_W :
			if (!ImGuizmo::IsUsing())
				m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
			break;

		case EZZOO_KEY_E:
			if (!ImGuizmo::IsUsing())
				m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case EZZOO_KEY_R:
			if (!ImGuizmo::IsUsing())
				m_GuizmoType = ImGuizmo::OPERATION::SCALE;
			break;

		default :
			break;

		}
		//return true;
	}

	void Sandbox2D::OnOverLayUpdate()
	{

		if (m_SceneStatus == SceneStatus::Play)
		{

			Entity entity = m_ActiveScene->GetCameraPrimaryEntity();

			if (!entity) return;
			Renderer2D::BeginScene(entity.GetComponent<CameraComponent>().Camera, entity.GetComponent<TransformComponent>().GetTransform());
		}
		
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);

		}
		
		if (m_ShowPhysicsColliders)
		{
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleColliderComponent>();

				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleColliderComponent>(entity);

					std::array<glm::vec3, 2> translationArray;
					translationArray[0] = tc.Translation + glm::vec3(cc2d.Offset, -0.001f);
					translationArray[1] = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);


					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Raduis * 2.0f);

					for (auto& trans : translationArray)
					{
						glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans) * glm::scale(glm::mat4(1.0f), scale);
						Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.03f);

					}

				}
			}

			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxColliderComponent>();

				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxColliderComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), scale);


					Renderer2D::SetLineWidth(1.0f);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));

				}
			}
		}
		Renderer2D::EndScene();

	}




	void Sandbox2D::OnDuplicate()
	{
		if (m_SceneStatus != SceneStatus::Edit)
			return;

		Entity selectedEntity = m_ScenePanel.GetSelectedEntity();

		if (selectedEntity)
		{
			Entity entity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_ScenePanel.SetSelectedEntity(entity);
		}
		
	}

	bool Sandbox2D::OnMOusePressedEvent(MousePressedEvent& event)
	{
		if (event.GetMouseButton() == EZZOO_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewPortHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(EZZOO_KEY_LEFT_ALT))
			{
				m_ScenePanel.SetSelectedEntity(m_HoveredEntity);
				return true;
			}
		}

		return false;
	}


	void Sandbox2D::OnEvent(Event& e)
	{
		//m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(EZZOO_BIND(Sandbox2D::OnKeyPressedEvent));
		dispatcher.Dispatch<MousePressedEvent>(EZZOO_BIND(Sandbox2D::OnMOusePressedEvent));

	}
}