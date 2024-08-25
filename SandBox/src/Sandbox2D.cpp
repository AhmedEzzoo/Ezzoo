
#include "Sandbox2D.h"

#include <cmath>
#include "Ezzoo/Core/EntryPoint.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1200.0f / 700.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Ezzoo::Texture2D::CreateTexture2D("assets/pic_1.png");

	m_ActiveScene = Ezzoo::CreateRef<Ezzoo::Scene>();

	Ezzoo::FrameBufferSpecification specs;
	specs.Width = 1248;
	specs.Height = 750;
	specs.Attachments = { Ezzoo::FrameBufferTextureFormate::RGBA8, Ezzoo::FrameBufferTextureFormate::RED_INTEGER, Ezzoo::FrameBufferTextureFormate::Depth };
	m_FrameBuffer = Ezzoo::FrameBuffer::Create(specs);

	m_GreenSquareEntity  =  m_ActiveScene->CreateEntity("Green Square");
	m_GreenSquareEntity.AddComponent<Ezzoo::SpriteRendererComponent>(glm::vec4{0.2f, 0.8f, 0.3f, 1.0f});
	
	m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
	m_CameraEntity.AddComponent<Ezzoo::CameraComponent>();

	m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip Space Camera");
	auto& cc = m_SecondCameraEntity.AddComponent<Ezzoo::CameraComponent>();
	cc.PrimaryCamera = false;
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Ezzoo::TimeStep ts)
{ 
	m_FrameBuffer->Bind();
	Ezzoo::Renderer2D::ResetStats();

	EZZOO_FUNCTION_SCOPE("Sandbox2D::OnUpdate");

	m_CameraController.OnUpdate(ts);

	Ezzoo::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ezzoo::RendererCommand::ClearColor();

	m_ActiveScene->OnUpdateEditor(ts);

	m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);

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
		if (ImGui::BeginMenu("Options"))
		{
			
			if (ImGui::MenuItem("Exit", NULL))
				Ezzoo::Application::GetApplication().Close();

			
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");


	ImGui::Text("Quad Indices Number: %d", Ezzoo::Renderer2D::GetStats().GetIndeciesCount());
	ImGui::Text("Quad Vertex Number: %d", Ezzoo::Renderer2D::GetStats().GetVertexCount());
	ImGui::Text("Draw Calls: %d", Ezzoo::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad Numbers: %d", Ezzoo::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Entity Name: %s", m_GreenSquareEntity.GetComponent<Ezzoo::TagComponent>().TagName.c_str());

	auto& color = m_GreenSquareEntity.GetComponent<Ezzoo::SpriteRendererComponent>().Color;
	ImGui::ColorEdit4("Square Color", glm::value_ptr(color));




	ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<Ezzoo::TransformComponent>().Transform[3]), 0.1f, 0.0f, 0.0f, "%.2f");

	if (ImGui::Checkbox("Primary Camera", &m_PrimaryCamera))
	{
		m_CameraEntity.GetComponent<Ezzoo::CameraComponent>().PrimaryCamera = m_PrimaryCamera;
		m_SecondCameraEntity.GetComponent<Ezzoo::CameraComponent>().PrimaryCamera = !m_PrimaryCamera;
	}


	ImGui::End();



	ImGui::Begin("ViewPort");

	ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewPortSize != *((glm::vec2*)&viewPortPanelSize) && viewPortPanelSize.x > 0 && viewPortPanelSize.y > 0)
	{
		m_FrameBuffer->Resize((uint32_t)viewPortPanelSize.x, (uint32_t)viewPortPanelSize.y);
		m_ViewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };
	}
	uint32_t id = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)id, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y });



	ImGui::End();
	ImGui::End();


}

void Sandbox2D::OnEvent(Ezzoo::Event& e)
{
	m_CameraController.OnEvent(e);
}
