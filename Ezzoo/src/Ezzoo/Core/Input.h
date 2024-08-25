#pragma once
#include "Ezzoo/Core/MouseCodes.h"
#include "Ezzoo/Core/KeyCodes.h"


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