#include "ezpch.h"

#include "Renderer2D.h"
#include "RendererCommand.h"

#include "VertexArray.h"



#include "Platform/OpenGL/OpenGLShader.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Ezzoo {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextCoords;
		float TextSlot = 0.0f;
		float TilingFactor = 1.0f;
	};

	struct Renderer2DData
	{

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndecies = MaxQuads * 6;

		static const uint32_t MaxTexturesSlots = 32;

		Ref<VertexArray> RendererVA;
		Ref<VertexBuffer> RendererVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexPtr = nullptr;

		uint32_t QuadIndciesCount = 0;

		std::array<Ref<Texture2D>, MaxTexturesSlots> Textures;

		uint32_t TextureIndex = 1;

		Renderer2D::RendererStats stats;

		glm::vec4 QuadPosition[4];
		
	};


	static Renderer2DData s_Data;

	Renderer2D::RendererStats& Renderer2D::GetStats() { return s_Data.stats; }

	void Renderer2D::Init()
	{
		s_Data.RendererVA = VertexArray::Create();
		s_Data.RendererVA->Bind();



		s_Data.QuadPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };


		s_Data.RendererVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.RendererVB->SetLayout({
				{ ShaderDataType::Float3, "a_Position"}, 
				{ ShaderDataType::Float4, "a_Color"}, 
				{ ShaderDataType::Float2, "a_TextCoord"},
				{ ShaderDataType::Float , "a_TextSlot"},
				{ ShaderDataType::Float , "a_TilingFactor"}

			});

		s_Data.RendererVA->AddVertexBuffer(s_Data.RendererVB);
		
		s_Data.QuadVertexBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* Quadindecies = new uint32_t[s_Data.MaxIndecies];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndecies; i += 6)
		{
			Quadindecies[i + 0] = offset + 0;
			Quadindecies[i + 1] = offset + 1;
			Quadindecies[i + 2] = offset + 2;

			Quadindecies[i + 3] = offset + 2;
			Quadindecies[i + 4] = offset + 3;
			Quadindecies[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(Quadindecies, s_Data.MaxIndecies / sizeof(uint32_t));

		s_Data.RendererVA->SetIndexBuffer(indexBuffer);
		delete[] Quadindecies;


		s_Data.WhiteTexture = Texture2D::CreateTexture2D(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t sampler[s_Data.MaxTexturesSlots];
		for (uint32_t i = 0; i < s_Data.MaxTexturesSlots; i++)
			sampler[i] = i;

		s_Data.TextureShader = Shader::Create("../SandBox/assets/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArr("u_Texture", sampler, s_Data.MaxTexturesSlots);
		

		//s_Data.QuadSader = Ezzoo::Shader::Create("../SandBox/assets/QuadShader.glsl");
		s_Data.Textures[0] = s_Data.WhiteTexture;


	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		//s_Data.ViewProjection = camera.GetViewProjection();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.QuadIndciesCount = 0;
		s_Data.QuadVertexPtr = s_Data.QuadVertexBase;
	}
	
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndciesCount = 0;
		s_Data.QuadVertexPtr = s_Data.QuadVertexBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexPtr - (uint8_t *)s_Data.QuadVertexBase;
		s_Data.RendererVB->SetData(s_Data.QuadVertexBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		//uint32_t count = s_Data.MaxIndecies / sizeof(uint32_t);
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
			s_Data.Textures[i]->Bind(i);

		RendererCommand::DrawIndexed(s_Data.RendererVA, s_Data.QuadIndciesCount);
		
			s_Data.stats.DrawCalls++;
			//EZZOO_CLIENT_INFO("{0}", s_Data.stats.DrawCalls);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const float texIndex = 0.0f;

		s_Data.QuadVertexPtr->Position = position;
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;
		
		s_Data.QuadVertexPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;


		s_Data.QuadIndciesCount += 6;
		s_Data.stats.QuadCount++;

		/*s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", color);
		
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.RendererVA->Bind();
		RendererCommand::DrawIndexed(s_Data.RendererVA);*/
	}


	void  Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, texture);
	}


	void  Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		float textIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
		{
			if (*s_Data.Textures[i].get() == *texture.get())
			{
				textIndex = (float)i;
				break;
			}
		}

		if (textIndex == 0.0f)
		{
			textIndex = (float)s_Data.TextureIndex;
			s_Data.Textures[s_Data.TextureIndex] = texture;
			s_Data.TextureIndex++;
		}



		s_Data.QuadVertexPtr->Position = position;
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexPtr++;


		s_Data.QuadIndciesCount += 6;
		s_Data.stats.QuadCount++;
		
		
		/*s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.RendererVA->Bind();
		RendererCommand::DrawIndexed(s_Data.RendererVA);*/
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		const float texIndex = 0.0f;
		const float tiligFactor = 1.0;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[0];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[1];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[2];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[3];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;


		s_Data.QuadIndciesCount += 6;
		s_Data.stats.QuadCount++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tiligFactor, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, tiligFactor, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tiligFactor, const glm::vec4& color)
	{

		const float texIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexPtr->Position = transform* s_Data.QuadPosition[0];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[1];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[2];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[3];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = texIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;


		s_Data.QuadIndciesCount += 6;
		s_Data.stats.QuadCount++;
	#if 0

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", color);

		s_Data.TextureShader->SetFloat("u_TilingFactor", tiligFactor);

		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.RendererVA->Bind();
		RendererCommand::DrawIndexed(s_Data.RendererVA);
	#endif
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, float tiligFactor, const Ref<Texture2D>& texture)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, tiligFactor, texture);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, float tiligFactor, const Ref<Texture2D>& texture)
	{

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
		{
			if (*s_Data.Textures[i].get() == *texture.get())
			{
				textIndex = (float)i;
				break;
			}
		}

		if (textIndex == 0.0f)
		{
			textIndex = (float)s_Data.TextureIndex;
			s_Data.Textures[s_Data.TextureIndex] = texture;
			s_Data.TextureIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[0];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[1];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 0.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[2];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 1.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadPosition[3];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TextCoords = { 0.0f, 1.0f };
		s_Data.QuadVertexPtr->TextSlot = textIndex;
		s_Data.QuadVertexPtr->TilingFactor = tiligFactor;
		s_Data.QuadVertexPtr++;


		s_Data.QuadIndciesCount += 6;
		s_Data.stats.QuadCount++;



	#if 0
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data.TextureShader->SetFloat("u_TilingFactor", tiligFactor);

		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.RendererVA->Bind();
		RendererCommand::DrawIndexed(s_Data.RendererVA);

	#endif
	}


	void Renderer2D::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(RendererStats));
	}

}