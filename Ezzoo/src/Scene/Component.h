#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Ezzoo/Core/TimeStep.h"
#include "Ezzoo/Renderer/Texture.h"
#include "Ezzoo/Core/UUID.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


namespace Ezzoo {


	struct IDComponent
	{
		UUID ID; 


		IDComponent() = default;
		IDComponent(const IDComponent& other) = default;
		IDComponent(const UUID& id)
			: ID(id)
		{

		}
		
	};

	struct TagComponent
	{
		std::string TagName;

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string & name) : TagName(name)
		{}


	};


	struct TransformComponent
	{
		glm::vec3 Translation { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale { 1.0f, 1.0f, 1.0f };

		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Translation) * ;


		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		//TransformComponent(const glm::mat4& trans) : Transform(trans)
		//{}
		TransformComponent(const glm::vec3& translation) : Translation(translation)
		{
		}
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * 
				rotation * 
				glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct CircleComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float Thickness = 1.0f;
		float Fade = 0.005f;
		

		CircleComponent() = default;
		CircleComponent(const CircleComponent& other) = default;
	};

	struct CameraComponent
	{

		Ezzoo::SceneCamera Camera;

		bool PrimaryCamera = true;
		bool FixedAspectRation = false;


		CameraComponent() = default;
		CameraComponent(const CameraComponent & other) = default;
	};

	struct ScriptComponent
	{

		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) = default;
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;


		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}


	};

	struct RigidBodyComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		void* RunTimeBody = nullptr;

		bool FixedRotation = false;

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;

	};

	struct BoxColliderComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Denisty = 1.0f;
		float Friction = 0.5f;
		float Ristitution = 0.0f;
		float RistitutionThreshold = 0.5f;


		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent&) = default;
	};


	struct CircleColliderComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Raduis = 0.5f;

		float Denisty = 1.0f;
		float Friction = 0.5f;
		float Ristitution = 0.0f;
		float RistitutionThreshold = 0.5f;


		CircleColliderComponent() = default;
		CircleColliderComponent(const CircleColliderComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//NativeScriptComponent() = default;

		std::function<void(void)> InstantiateScript ;
		std::function<void(void)> DestroyInstanceFunc;

/*

		std::function<void(ScriptableEntity* script)> OnCreateFunc;
		std::function<void(ScriptableEntity* script)> OnDestroyFunc;
		std::function<void(ScriptableEntity* script, TimeStep ts)> OnUpdateFunc;*/


		template <typename T>
		void Bind()
		{
			InstantiateScript = [&]() { Instance = new T(); };
			DestroyInstanceFunc = [&]() { delete Instance;  Instance = nullptr; };

		}

	};

	template <typename... Components>
	struct ComponentsGroup
	{

	};

	using AllComponents = ComponentsGroup<TransformComponent, ScriptComponent, SpriteRendererComponent, CameraComponent, CircleComponent, BoxColliderComponent, RigidBodyComponent, CircleColliderComponent>;
}