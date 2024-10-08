#pragma once

#include "Ezzoo/Renderer/Camera.h"

namespace Ezzoo {

	class SceneCamera : public Camera
	{
	public :

		SceneCamera();
		virtual ~SceneCamera() = default;



		void SetOrthogrphic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t  width, uint32_t height);

	private :
		void RecalculateMatrix();

	private :

		float m_OrthographicSize = 10.0f;

		float m_NearClip = -1.0f, m_FarClip = 1.0f;

		float m_AspectRatio = 0.0f;

	};
}