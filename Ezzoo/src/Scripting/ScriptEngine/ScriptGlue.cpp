#include "ezpch.h"

#include "ScriptGlue.h"

#include "mono/jit/jit.h"

#include "Scene/Component.h"
#include "ScriptEngine.h"

#include "Ezzoo/Core/Input.h"
#include "box2d/b2_body.h"

#define EZZOO_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ezzoo.InternalCalls::"#Name, Name);

namespace Ezzoo {


	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFunc;

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static float NativeVectorLog(glm::vec3* vec3)
	{
		EZZOO_CORE_TRACE("{0}", *vec3);

		float x = glm::dot(*vec3, *vec3);

		return x;
	}

	static glm::vec3* NativeVectorCross(glm::vec3* vec3, glm::vec3* rVec)
	{
		*rVec = glm::cross(*vec3, glm::vec3(vec3->x, vec3->y, -vec3->z));

		return rVec;
	}

	static void TransformComponent_GetTranslation(UUID id, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetActiveContext();
		Entity entt = scene->GetEntityByID(id);

		*outTranslation = entt.GetComponent<TransformComponent>().Translation;
		
	}

	static void TransformComponent_SetTranslation(UUID id, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetActiveContext();
		Entity entt = scene->GetEntityByID(id);

		entt.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Entity_IsKeyDown(int code)
	{
		return Input::IsKeyPressed(code);
	}



	static bool Entity_HasComponent(UUID id, MonoReflectionType* reflectionType)
	{

		Scene* scene = ScriptEngine::GetActiveContext();
		Entity ent = scene->GetEntityByID(id);

		MonoType* componentType = mono_reflection_type_get_type(reflectionType);
		return s_EntityHasComponentFunc.at(componentType)(ent);

	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([&]() {
			std::string_view name = typeid(Component).name();
			size_t pos = name.find_last_of(":");
			std::string_view cTypeName = name.substr(pos + 1);
			std::string typeName = fmt::format("Ezzoo.{}", cTypeName);

			MonoType* monoType = mono_reflection_type_from_name(typeName.data(), ScriptEngine::GetMonoImage());
			s_EntityHasComponentFunc[monoType] = [](Entity ent) { return ent.HasComponent<Component>(); };
			}(), ...);
	}

	static void Entity_ApplyLinearImpulse(UUID id, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetActiveContext();
		Entity ent = scene->GetEntityByID(id);

		auto& rbc = ent.GetComponent<RigidBodyComponent>();

		b2Body* body =  (b2Body*)rbc.RunTimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}


	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent<TransformComponent, ScriptComponent, SpriteRendererComponent, CircleComponent, RigidBodyComponent, CameraComponent, BoxColliderComponent, CircleColliderComponent>();
		
	}



	void ScriptGlue::InternalCalls()
	{
		EZZOO_ADD_INTERNAL_CALL(NativeLog);
		EZZOO_ADD_INTERNAL_CALL(NativeVectorLog);
		EZZOO_ADD_INTERNAL_CALL(NativeVectorCross);

		EZZOO_ADD_INTERNAL_CALL(Entity_IsKeyDown);

		EZZOO_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		EZZOO_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EZZOO_ADD_INTERNAL_CALL(Entity_ApplyLinearImpulse);

		EZZOO_ADD_INTERNAL_CALL(Entity_HasComponent);
	}



}