#pragma once
#include "Ezzoo/MouseCodes.h"
#include "Ezzoo/KeyCodes.h"


namespace Ezzoo {

	class Input
	{
	public :
		static bool IsKeyPressed(int keyCode);
		static bool IsMousePressed(MouseCodes mouseCode);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}