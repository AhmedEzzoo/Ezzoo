#include "ezpch.h"

#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ezzoo {


	SceneCamera::SceneCamera()
	{
		RecalculateMatrix();
	}


	void SceneCamera::SetOrthogrphic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;

		m_NearClip = nearClip;
		m_FarClip = farClip;
		RecalculateMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t  width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateMatrix();
	}

	void SceneCamera::RecalculateMatrix()
	{
		float left = -0.5f * m_AspectRatio * m_OrthographicSize;
		float right = 0.5f * m_AspectRatio * m_OrthographicSize;
		float bottom = -0.5f * m_OrthographicSize;
		float top = 0.5f * m_OrthographicSize;


		m_Projection = glm::ortho(left, right, bottom, top, m_NearClip, m_FarClip);
	}



}