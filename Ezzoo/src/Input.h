#pragma once

namespace Ezzoo {

	class Input
	{
	public :
		static bool IsKeyPressed(int keyCode);
		static bool IsMousePressed(int mouseCode);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}