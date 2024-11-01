#include "ezpch.h"
#include "Component.h"
#include "Entity.h"
#include "Ezzoo/Renderer/Renderer2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_shape.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#include "Scripting/ScriptEngine/ScriptEngine.h"


namespace Ezzoo {

	std::unordered_map<UUID, entt::entity> Scene::m_EnttMap = {};


	static b2BodyType RigidBodyComponentTypeToBox2DType(RigidBodyComponent component)
	{
		switch (component.Type)
		{
		case RigidBodyComponent::BodyType::Static: return b2BodyType::b2_staticBody;
		case RigidBodyComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
		case RigidBodyComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
		}

		EZZOO_CORE_ASSERT(false, "Unknown Body Type");
		return b2_staticBody;
	}


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

	template <typename Component>
	static void CopyComponent(entt::registry& dstReg, entt::registry& srcReg, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = srcReg.view<Component>();
		for (auto entt : view)
		{
			//EZZOO_CORE_ASSERT((enttMap.find(uuid) != enttMap.end()), "ID Does not exist");
			entt::entity e = enttMap.at(srcReg.get<IDComponent>(entt).ID);
			auto& component = srcReg.get<Component>(entt);

			dstReg.emplace_or_replace<Component>(e, component);
		}

	
	}

	
	template <typename... Component>
	static void CopyComponentIfExist(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}


	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		//std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcRegister = other->m_Registry;
		auto& dstRegister = newScene->m_Registry;

		auto srcView = srcRegister.view<IDComponent>();

		for (auto e : srcView)
		{
			UUID id = srcRegister.get<IDComponent>(e).ID;
			const auto& name = srcRegister.get<TagComponent>(e).TagName;

			Entity entt = newScene->CreateEntityWithID(id, name);
			m_EnttMap[id] = (entt::entity)entt;
		}



		CopyComponent<TransformComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<ScriptComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<SpriteRendererComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<CircleComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<CameraComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<RigidBodyComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<BoxColliderComponent>(dstRegister, srcRegister, m_EnttMap);
		CopyComponent<CircleColliderComponent>(dstRegister, srcRegister, m_EnttMap);

		return newScene;
	
	}

	Entity Scene::DuplicateEntity(Entity src)
	{
		Entity newEntity = CreateEntity(src.GetComponent<TagComponent>().TagName);

		CopyComponentIfExist<TransformComponent, ScriptComponent,  SpriteRendererComponent, CircleComponent, RigidBodyComponent, CameraComponent, BoxColliderComponent, CircleColliderComponent>(newEntity, src);

		return newEntity;

	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(const UUID& id, const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.TagName = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
	void Scene::OnRunTimeStart()
	{
		OnPhysics2DRender();

		ScriptEngine::OnRunTimeStart(this);
		auto scriptViewEntities = m_Registry.view<ScriptComponent>();
		for (auto entity : scriptViewEntities)
		{
			Entity ent{ entity, this };
			ScriptEngine::OnCreate(ent);
		}
	}

	void Scene::OnRunTimeStop()
	{
		delete m_Physicsworld;
		ScriptEngine::OnRunTimeStop();
	
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DRender();

	}


	void Scene::OnSimulationStop()
	{
		delete m_Physicsworld;

	}
	
	void Scene::OnPhysics2DRender()
	{
		m_Physicsworld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<RigidBodyComponent>();
		for (auto e : view)
		{
			Entity ent{ e, this };

			auto& trans = ent.GetComponent<TransformComponent>();
			auto& b2rbc = ent.GetComponent<RigidBodyComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = RigidBodyComponentTypeToBox2DType(b2rbc);
			bodyDef.position.Set(trans.Translation.x, trans.Translation.y);
			bodyDef.angle = trans.Rotation.z;

			b2Body* body = m_Physicsworld->CreateBody(&bodyDef);
			body->SetFixedRotation(b2rbc.FixedRotation);
			b2rbc.RunTimeBody = (void*)body;


			if (ent.HasComponent<BoxColliderComponent>())
			{
				auto& boxCollider = ent.GetComponent<BoxColliderComponent>();

				b2PolygonShape shape;
				shape.SetAsBox(boxCollider.Size.x * trans.Scale.x, boxCollider.Size.y * trans.Scale.y);

				b2FixtureDef fixDef;
				fixDef.shape = &shape;
				fixDef.density = boxCollider.Denisty;
				fixDef.friction = boxCollider.Friction;
				fixDef.restitution = boxCollider.Ristitution;
				fixDef.restitutionThreshold = boxCollider.RistitutionThreshold;

				body->CreateFixture(&fixDef);
			}

			if (ent.HasComponent<CircleColliderComponent>())
			{
				auto& cCollider = ent.GetComponent<CircleColliderComponent>();

				b2CircleShape shape;
				shape.m_p.Set(cCollider.Offset.x, cCollider.Offset.y);
				shape.m_radius = cCollider.Raduis * trans.Scale.x;

				b2FixtureDef fixDef;
				fixDef.shape = &shape;
				fixDef.density = cCollider.Denisty;
				fixDef.friction = cCollider.Friction;
				fixDef.restitution = cCollider.Ristitution;
				fixDef.restitutionThreshold = cCollider.RistitutionThreshold;

				body->CreateFixture(&fixDef);
			}



		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{

		Renderer2D::BeginScene(camera);

		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{

				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				//EZZOO_CLIENT_WARNING("{0}", sprite.Color.x);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite, (int)entity);
			}
		}

		{

			auto view = m_Registry.view<TransformComponent, CircleComponent>();
			for (auto entity : view)
			{

				auto [transform, circle] = view.get<TransformComponent, CircleComponent>(entity);
				//EZZOO_CLIENT_WARNING("{0}", sprite.Color.x);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade , (int)entity);
			}

		}


		//Renderer2D::DrawLine(glm::vec3(1.0f), glm::vec3(2.0f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f ));
		//Renderer2D::DrawPoints(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		
		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRunTime(TimeStep ts)
	{

		//C# Script 
		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				Entity ent{ entity, this };
				ScriptEngine::OnUpdate(ent, ts);
			}

		}




		//Native Script

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
	
		//Physics
			const int32_t positionIteration = 2;
			const int32_t velocityIteration = 6;
			m_Physicsworld->Step(ts, velocityIteration, positionIteration);

			auto view = m_Registry.view<RigidBodyComponent>();
			for (auto entt : view)
			{
				Entity ent{ entt, this };
				auto& trans = ent.GetComponent<TransformComponent>();
				auto& rigidBodyCompenent = ent.GetComponent<RigidBodyComponent>();

				b2Body* body = (b2Body*)rigidBodyCompenent.RunTimeBody;
				const auto& position = body->GetPosition();
				trans.Translation.x = position.x;
				trans.Translation.y = position.y;
				trans.Rotation.z = body->GetAngle();
			}
		


		//Renderer 2D
		Ezzoo::Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{

			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				auto [trans, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.PrimaryCamera)
				{
					mainCamera = &camera.Camera;
					cameraTransform = trans.GetTransform();
					break;
				}
			}

		}

		

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [trans, color] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(trans.GetTransform(), color, (int)entity);
				}

			}

			{
				auto view = m_Registry.view<TransformComponent, CircleComponent>();
				for (auto entity : view)
				{
					auto [trans, circle] = view.get<TransformComponent, CircleComponent>(entity);

					Renderer2D::DrawCircle(trans.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}

			}
			
			Renderer2D::EndScene();
		}
	
		
	}





	void Scene::OnUpdateSimulate(TimeStep ts, EditorCamera& camera)
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

		//Physics



		const int32_t positionIteration = 2;
		const int32_t velocityIteration = 6;

		m_Physicsworld->Step(ts, velocityIteration, positionIteration);
		auto view = m_Registry.view<RigidBodyComponent>();
		for (auto entt : view)
		{
			Entity ent{ entt, this };
			auto& trans = ent.GetComponent<TransformComponent>();
			auto& rigidBodyCompenent = ent.GetComponent<RigidBodyComponent>();

			b2Body* body = (b2Body*)rigidBodyCompenent.RunTimeBody;
			const auto& position = body->GetPosition();
			trans.Translation.x = position.x;
			trans.Translation.y = position.y;
			trans.Rotation.z = body->GetAngle();
		}

		Renderer2D::BeginScene(camera);

		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{

				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				//EZZOO_CLIENT_WARNING("{0}", sprite.Color.x);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite, (int)entity);
			}
		}

		{

			auto view = m_Registry.view<TransformComponent, CircleComponent>();
			for (auto entity : view)
			{

				auto [transform, circle] = view.get<TransformComponent, CircleComponent>(entity);
				//EZZOO_CLIENT_WARNING("{0}", sprite.Color.x);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}

		}

		Renderer2D::EndScene();
	}


	Entity Scene::GetEntityByID(UUID id)
	{
		//if (m_EnttMap != m_EnttMap.end())
		return {};
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
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetCameraPrimaryEntity()
	{
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);

			if (camera.PrimaryCamera)
			{
				return Entity { entity, this };

			}
		}

		return {};

	}


	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		//static_assert(true);
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component)
	{
	}
	template <>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template <>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, CircleComponent& component)
	{
	}
	template <>
	void Scene::OnComponentAdded(Entity entity, CircleColliderComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component)
	{
	}
	template <>
	void Scene::OnComponentAdded(Entity entity, ScriptComponent& component)
	{
	}
	template <>
	void Scene::OnComponentAdded(Entity entity, RigidBodyComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, BoxColliderComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded(Entity entity, IDComponent& component)
	{
	}


}