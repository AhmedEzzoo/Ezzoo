#include "Ezzoo.h"

class SandBox : public Ezzoo::Application
{
public :
	SandBox()
	{

	}

	~SandBox()
	{

	}
};

Ezzoo::Application* CreatApplication()
{
	return new SandBox();
}
