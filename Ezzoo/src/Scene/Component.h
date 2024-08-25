#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneCamera.h"


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
		glm::mat4 Transform = glm::mat4{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::mat4& trans) : Transform(trans)
		{}

		operator glm::mat4()
		{
			return Transform;
		}
		operator glm::mat4() const 
		{
			return Transform;
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
}