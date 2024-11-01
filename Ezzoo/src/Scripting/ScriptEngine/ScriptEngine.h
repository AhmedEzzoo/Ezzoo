#pragma once
#include <filesystem>

#include "Scene/Scene.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoMethod MonoMethod;

}


namespace Ezzoo {

	class ScriptEngine
	{

	public :

		static void Init();
		static void Shutdown();

		static void  LoadAssembly(const std::filesystem::path& path);
		
		static void OnRunTimeStart(Scene* scene);
		static void OnRunTimeStop();
		static void OnCreate(Entity entity);
		static void OnUpdate(Entity entity, TimeStep ts);
		static Scene* GetActiveContext()  { return m_ActiveScene; }
	private :
		static MonoObject* Instantiate(MonoClass* className);
		static void InitMono();
		static void ShutdownMono();
		static MonoImage* GetImageAssy(MonoAssembly* coreAssy);
		
	
		static void LoadAssymblyClasses();
	private :
		static Scene* m_ActiveScene;


		friend class ScriptClass;

	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass>& scriptClass, Entity entity);
		void InvokeOnCreate();
		void InvokeOnUpdate(TimeStep ts);

	private:
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		Ref<ScriptClass> m_SClass;
	};

	class ScriptClass
	{
	public :
		ScriptClass() = default;

		ScriptClass(const std::string& namespaceName, const std::string& className);
		MonoMethod* GetMethodByName(const std::string& methodName, int params);
		void Invoke(MonoMethod* method, MonoObject* instance, void** params);
		MonoObject* InstantiateClass();

		MonoClass* GetClass() const { return m_Class; }

	private :
		MonoClass* m_Class = nullptr;
		//MonoObject* m_ClassInstance = nullptr;

		std::string m_NameSpace;
		std::string m_Name;

		void** m_Params = nullptr;
		
	};
}