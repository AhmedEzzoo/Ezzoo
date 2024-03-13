#include "Ezzoo.h"


class ExampleLayer : public Ezzoo::Layer
{

public :

	ExampleLayer() : Layer("ExampleLayer") {}
	~ExampleLayer () {}
	virtual void OnAttach() override
	{

	}
	virtual void OnDetach() override 
	{

	}
	virtual void OnUpdate() override 
	{
		//EZZOO_CLIENT_INFO("Example Layer Update");
	}
	virtual void OnEvent(Ezzoo::Event& event) override 
	{
		//EZZOO_CLIENT_TRACE("Example Layer Event {0}", event);
	}

private :

};



class SandBox : public Ezzoo::Application
{
public :
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverLayer(new Ezzoo::ImGuiLayer());
	}

	~SandBox()
	{

	}
};

Ezzoo::Application* CreatApplication()
{
	return new SandBox();
}
