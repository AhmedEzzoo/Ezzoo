#pragma once

#include "OrthoGraphicCamera.h"
#include "Camera.h"
#include "Texture.h"

namespace Ezzoo {


	class Renderer2D
	{
	public :
		
		static void Init();
		static void ShutDown();


		static void BeginScene(const OrthoGraphicCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);


		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tiligFactor, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tiligFactor, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tiligFactor, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tiligFactor, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::mat4& transform,  const glm::vec4& color);

		struct RendererStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetIndeciesCount() { return QuadCount * 6; }
			uint32_t GetVertexCount() { return QuadCount * 4; }
		};

		static RendererStats& GetStats();

		static void ResetStats();





	private :

	};
}