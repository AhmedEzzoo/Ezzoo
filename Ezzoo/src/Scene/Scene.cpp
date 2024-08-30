#include "ezpch.h"
#include "Component.h"
#include "Entity.h"
#include "Ezzoo/Renderer/Renderer2D.h"

namespace Ezzoo {

	Scene::Scene()
	{

		/*entt::entity ent = m_Registry.create();

		TransformComponent& transfom = m_Registry.emplace<TransformComponent>(ent);

		m_Registry.get<TransformComponent>(ent);

		auto view = m_Registry.view<TransformComponent>();

		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);

		}*/
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name )
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);


		return entity;
	}

	void Scene::OnUpdateEditor(TimeStep ts)
	{
		{

			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {

				if (!nsc.Instance)
				{
					nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);

				});

		}
		Ezzoo::Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{

			auto view = m_Registry.view<CameraComponent, TransformComponent>();

			for (auto entity : view)
			{
				auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

				if (camera.PrimaryCamera)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}

		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, color] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, color.Color);
			}
			Renderer2D::EndScene();
		}
	
		
	}

	void Scene::OnUpdateScript(TimeStep ts)
	{
	
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{		
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRation)
			{
				cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
			}
		}
	}

}