#pragma once


#include "Ezzoo/Events/Event.h"
namespace Ezzoo {
	
	
	class Layer
	{
	public :

		Layer(const std::string& title = "Layer");
		virtual~Layer();

		virtual void OnAttach () {}
		virtual void OnDetach() {}
		virtual void OnUpdate () {}
		virtual void OnEvent (Event & event){}

		inline  const std::string& GetName() const { return m_Title; }


	private:
		std::string m_Title;
	};

}

