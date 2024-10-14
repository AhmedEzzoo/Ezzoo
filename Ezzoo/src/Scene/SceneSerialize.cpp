#include "ezpch.h"

#include "SceneSerialize.h"
#include "Entity.h"
#include "Component.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include <Windows.h>
#include <commdlg.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Ezzoo/Core/Application.h"

namespace YAML
{

	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}




namespace Ezzoo {



	static  std::string RigidBodyTypeToString(RigidBodyComponent::BodyType type)
	{
		switch (type)
		{
		case RigidBodyComponent::BodyType::Static:		return "Static";
		case RigidBodyComponent::BodyType::Dynamic:		return "Dynamic";
		case RigidBodyComponent::BodyType::Kinematic:	return "Kinematic";
		}

		return std::string();
	}

	static RigidBodyComponent::BodyType StringToRigidBodyType(const std::string& type)
	{
		if (type == "Static")			return RigidBodyComponent::BodyType::Static;
		else if (type == "Dynamic")		return RigidBodyComponent::BodyType::Dynamic;
		else if (type == "Kinematic")	return RigidBodyComponent::BodyType::Kinematic;

		return RigidBodyComponent::BodyType::Static;

	}

	SceneSerialize::SceneSerialize(const Ref <Scene>& scene) 
		: m_Scene(scene)
	{
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y  << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}	
	
	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}

	


	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		if (entity.HasComponent<IDComponent>()) 
			out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID;



		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			
			auto& tag = entity.GetComponent<TagComponent>().TagName;

			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "Translation" << YAML::Value << component.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << component.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleComponent>())
		{
			out << YAML::Key << "CircleComponent";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<CircleComponent>();

			out << YAML::Key << "Color" << YAML::Value << component.Color;
			out << YAML::Key << "Thickness" << YAML::Value << component.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << component.Fade;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleColliderComponent>())
		{
			out << YAML::Key << "CircleCollider 2D";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<CircleColliderComponent>();

			out << YAML::Key << "Offset" << YAML::Value << component.Offset;
			out << YAML::Key << "Raduis" << YAML::Value << component.Raduis;
			out << YAML::Key << "Density" << YAML::Value << component.Denisty;
			out << YAML::Key << "Friction" << YAML::Value << component.Friction;
			out << YAML::Key << "Ristitution" << YAML::Value << component.Ristitution;
			out << YAML::Key << "RistitutionThreshold" << YAML::Value << component.RistitutionThreshold;

			out << YAML::EndMap;
		}


		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;


			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PrespectiveFOV" << YAML::Value << camera.GetPrespectiveFov();
			out << YAML::Key << "PrespectiveNear" << YAML::Value << camera.GetPrespectiveNear();
			out << YAML::Key << "PrespectiveFar" << YAML::Value << camera.GetPrespectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.PrimaryCamera;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << component.Color;
			if (component.Texture)
				out << YAML::Key << "Texture" << YAML::Value << component.Texture->GetPath();
			else 
				out << YAML::Key << "Texture" << YAML::Value << "";
			out << YAML::Key << "TillingFactor" << YAML::Value << component.TilingFactor;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBodyComponent>())
		{
			out << YAML::Key << "RigidBody 2D";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<RigidBodyComponent>();

			out << YAML::Key << "Type" << YAML::Value << RigidBodyTypeToString(component.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << component.FixedRotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxColliderComponent>())
		{
			out << YAML::Key << "BoxCollider 2D";
			out << YAML::BeginMap;

			auto& component = entity.GetComponent<BoxColliderComponent>();

			out << YAML::Key << "Offset" << YAML::Value << component.Offset;
			out << YAML::Key << "Size" << YAML::Value << component.Size;
			out << YAML::Key << "Denisty" << YAML::Value << component.Denisty;
			out << YAML::Key << "Friction" << YAML::Value << component.Friction;
			out << YAML::Key << "Ristitution" << YAML::Value << component.Ristitution;
			out << YAML::Key << "RistitutionThreshold" << YAML::Value << component.RistitutionThreshold;
			out << YAML::EndMap;
		}
		
		out << YAML::EndMap;
	}


	void SceneSerialize::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled";

		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entity) {

			Entity entt = { entity, m_Scene.get() };

			if (!entt) return;
			SerializeEntity(out, entt);

			});


		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerialize::SerializeRunTime(const std::string& filepath)
	{
		return false;
	}

	bool SceneSerialize::Deserialize(const std::string& filepath)
	{
		
		YAML::Node data ;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			EZZOO_CORE_ERROR(e.what());
			return false;
		}


		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities) 
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				EZZOO_CORE_TRACE("UUID Value is : {0}, TagComponent Name : {1}", uuid, name);

				Entity deserialzedEntity = m_Scene->CreateEntityWithID(uuid, name);

				auto transformComponent = entity["TransformComponent"]; //TransformComponent
				if (transformComponent)
				{
					auto& tc = deserialzedEntity.GetComponent<TransformComponent>();

					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto CComponent = entity["CircleComponent"]; //TransformComponent
				if (CComponent)
				{
					auto& cc = deserialzedEntity.AddComponent<CircleComponent>();

					cc.Color = CComponent["Color"].as<glm::vec4>();
					cc.Thickness = CComponent["Thickness"].as<float>();
					cc.Fade = CComponent["Fade"].as<float>();
				}

				auto spritComponent = entity["SpriteRendererComponent"];
				if (spritComponent)
				{
					auto& sprite = deserialzedEntity.AddComponent<SpriteRendererComponent>();
					sprite.Color = spritComponent["Color"].as<glm::vec4>();
					const std::string& path = spritComponent["Texture"].as<std::string>();
					if (!path.empty())
					{
						sprite.Texture = Texture2D::CreateTexture2D(path);
					}
					
					if (spritComponent["TillingFactor"])
						sprite.TilingFactor = spritComponent["TillingFactor"].as<float>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserialzedEntity.AddComponent<CameraComponent>();
					const auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					
					cc.Camera.SetPrespectiveFov(cameraProps["PrespectiveFOV"].as<float>());
					cc.Camera.SetPrespectiveNear(cameraProps["PrespectiveNear"].as<float>());
					cc.Camera.SetPrespectiveFar(cameraProps["PrespectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());


					cc.PrimaryCamera = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRation = cameraComponent["FixedAspectRatio"].as<bool>(); 
				}

				auto RigidBody = entity["RigidBody 2D"];
				if (RigidBody)
				{
					auto& rbc = deserialzedEntity.AddComponent<RigidBodyComponent>();

					rbc.Type = StringToRigidBodyType(RigidBody["Type"].as<std::string>());
					rbc.FixedRotation = RigidBody["FixedRotation"].as<bool>();
				}

				auto BoxCol = entity["BoxCollider 2D"];
				if (BoxCol)
				{
					auto& bcc = deserialzedEntity.AddComponent<BoxColliderComponent>();

					bcc.Offset = BoxCol["Offset"].as<glm::vec2>();
					bcc.Size = BoxCol["Size"].as<glm::vec2>();
					bcc.Denisty = BoxCol["Denisty"].as<float>();
					bcc.Friction = BoxCol["Friction"].as<float>();
					bcc.Ristitution = BoxCol["Ristitution"].as<float>();
					bcc.RistitutionThreshold = BoxCol["RistitutionThreshold"].as<float>();
				}


				auto cCol = entity["CircleCollider 2D"];
				if (cCol)
				{
					auto& cc = deserialzedEntity.AddComponent<CircleColliderComponent>();

					cc.Offset = cCol["Offset"].as<glm::vec2>();
					cc.Raduis = cCol["Raduis"].as<float>();
					cc.Denisty = cCol["Density"].as<float>();
					cc.Friction = cCol["Friction"].as<float>();
					cc.Ristitution = cCol["Ristitution"].as<float>();
					cc.RistitutionThreshold = cCol["RistitutionThreshold"].as<float>();
				}
			
			}
		}
		return true;
	}

	bool SceneSerialize::DeserializeRunTime(const std::string& filepath)
	{
		EZZOO_CORE_ASSERT(!m_Scene, "Scene Doesnot Exist");

		return false;
	}


	std::string SceneSerialize::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}



	std::string SceneSerialize::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

}