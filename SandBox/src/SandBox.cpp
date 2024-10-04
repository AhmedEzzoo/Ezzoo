
#include "Sandbox.h"
#include "Ezzoo/Core/EntryPoint.h"
#if 0
class ExampleLayer : public Ezzoo::Layer
{

public :

	ExampleLayer() : Layer("ExampleLayer"), m_CameraController(1200.0f/ 700.0f, true) 
	{
		m_VertexArray = Ezzoo::VertexArray::Create();
		m_VertexArray->Bind();

		float vertices[5 * 4] = {
			 -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			0.75f, 0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f, 0.75f, 0.0f, 0.0f, 1.0f
		};
		m_VertexBuffer = Ezzoo::VertexBuffer::Create(vertices, sizeof(vertices));


		m_VertexBuffer->SetLayout({ {Ezzoo::ShaderDataType::Float3, "a_Position"},
								{Ezzoo::ShaderDataType::Float2, "a_TextCoord"} });

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);



		uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0};
		m_IndexBuffer = Ezzoo::IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		 

		//m_ChessBoardShader = Ezzoo::Shader::Create("assets/Texture.glsl");

		auto chessBoardShader = m_ShaderLib.Load("Texture","assets/Texture.glsl");

		m_Texture = Ezzoo::Texture2D::CreateTexture2D("assets/Pictures/pic_1.png");
		std::dynamic_pointer_cast<Ezzoo::OpenGLShader>(chessBoardShader)->UploadUniformInt("u_Texture", 0);


		auto m_LogoShader = m_ShaderLib.Load("Logo", "assets/Shaders/Logo.glsl");

		m_LogoTexture = Ezzoo::Texture2D::CreateTexture2D("assets/Pictures/pic_2.png");
		std::dynamic_pointer_cast<Ezzoo::OpenGLShader>(m_LogoShader)->UploadUniformInt("u_Texture", 0);

	}
	~ExampleLayer () {}
	virtual void OnAttach() override
	{

	}
	virtual void OnDetach() override 
	{

	}
	virtual void OnUpdate(Ezzoo::TimeStep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Ezzoo::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ezzoo::RendererCommand::ClearColor();

		Ezzoo::Renderer::BeginScene(m_CameraController.GetCamera());

		m_Texture->Bind();
		auto chessShader = m_ShaderLib.Get("Texture");
		Ezzoo::Renderer::Submit(m_VertexArray, chessShader);

		m_LogoTexture->Bind();
		auto logoShader = m_ShaderLib.Get("Logo");
		Ezzoo::Renderer::Submit(m_VertexArray, logoShader);

		Ezzoo::Renderer::EndScene();

		
	}
	virtual void OnImGuiRender() override 
	{
	
	}
	virtual void OnEvent(Ezzoo::Event& e) override 
	{
		m_CameraController.OnEvent(e);
	} 

private :

	Ezzoo::ShaderLibrary m_ShaderLib;
	Ezzoo::Ref<Ezzoo::VertexArray> m_VertexArray;
	Ezzoo::Ref<Ezzoo::VertexBuffer> m_VertexBuffer;
	Ezzoo::Ref<Ezzoo::IndexBuffer> m_IndexBuffer;

	Ezzoo::Ref<Ezzoo::Texture2D> m_Texture, m_LogoTexture;

	Ezzoo::OrthgraphicCameraController m_CameraController;

};
#endif

namespace Ezzoo {
	
	SandBox::SandBox(const ApplicationSpecification& specs)
		: Application (specs)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	SandBox::~SandBox()
	{

	}
}

Ezzoo::Application* Ezzoo::CreatApplication(ApplicationSpecification& specs)
{
	specs.Name = "SandBox";
	specs.path  = "../SandBox";


	return new SandBox(specs);
}



