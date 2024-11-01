#include "ezpch.h"

#include "ScriptGlue.h"

#include "mono/jit/jit.h"

#include "Scene/Component.h"
#include "ScriptEngine.h"

#define EZZOO_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ezzoo.InternalCalls::"#Name, Name);

namespace Ezzoo {

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

	static glm::vec3* Entity_GetTranslation(UUID id)
	{
		Scene* scene = ScriptEngine::GetActiveContext();
		return nullptr;
	}


	void ScriptGlue::InternalCalls()
	{
		EZZOO_ADD_INTERNAL_CALL(NativeLog);
		EZZOO_ADD_INTERNAL_CALL(NativeVectorLog);
		EZZOO_ADD_INTERNAL_CALL(NativeVectorCross);

		EZZOO_ADD_INTERNAL_CALL(Entity_GetTranslation);
	}



}