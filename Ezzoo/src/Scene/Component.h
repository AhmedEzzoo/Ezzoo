#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Ezzoo/Core/TimeStep.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


namespace Ezzoo {



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
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Translation) * ;


		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		//TransformComponent(const glm::mat4& trans) : Transform(trans)
		//{}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct CameraComponent
	{

		Ezzoo::SceneCamera Camera;

		bool PrimaryCamera = true;
		bool FixedAspectRation = false;


		CameraComponent() = default;
		CameraComponent(const CameraComponent & other) = default;
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}


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
}