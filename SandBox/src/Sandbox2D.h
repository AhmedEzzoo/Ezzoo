#pragma once

#include "Ezzoo.h"


class Sandbox2D : public Ezzoo::Layer
{

public :
	Sandbox2D();

	virtual ~Sandbox2D() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Ezzoo::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Ezzoo::Event& event) override;



private :
	Ezzoo::Ref<Ezzoo::Shader> m_QuadShader;
	Ezzoo::Ref<Ezzoo::VertexArray> m_VertexArray;
	Ezzoo::Ref<Ezzoo::VertexBuffer> m_VertexBuffer;
	Ezzoo::Ref<Ezzoo::IndexBuffer> m_IndexBuffer;
	Ezzoo::Ref<Ezzoo::Texture2D> m_Texture;

	Ezzoo::Ref<Ezzoo::FrameBuffer> m_FrameBuffer;

	Ezzoo::OrthgraphicCameraController m_CameraController;

	Ezzoo::Ref<Ezzoo::Scene> m_ActiveScene;

	Ezzoo::Entity m_GreenSquareEntity;
	Ezzoo::Entity m_CameraEntity;
	Ezzoo::Entity m_SecondCameraEntity;


	bool m_PrimaryCamera = true;

	glm::vec2 m_ViewPortSize = { 0.0f, 0.0f };
};