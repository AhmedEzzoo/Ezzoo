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
		if (event.GetEventType() == Ezzoo::EventType::KeyPressed)
		{
			if (Ezzoo::Input::IsKeyPressed((int)EZZOO_KEY_TAB))
				EZZOO_CLIENT_TRACE("Example Layer Tab Key Pressed : {0}", event);
		}
	}

private :

};



class SandBox : public Ezzoo::Application
{
public :
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox()
	{

	}
};

Ezzoo::Application* Ezzoo::CreatApplication()
{
	return new SandBox();
}
