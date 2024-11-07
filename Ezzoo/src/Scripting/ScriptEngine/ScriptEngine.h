#pragma once
#include <filesystem>

#include "Scene/Scene.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoClassField MonoClassField;

}


namespace Ezzoo {
	enum class ScriptFields
	{
		None = 0,
		Bool, Byte, Float, Int, Double, Long,
		UByte, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptFieldData
	{

		std::string Name;
		ScriptFields Field = ScriptFields::None;

		MonoClassField* ClassField = nullptr;
	};

	struct ScriptFieldInstance
	{
		ScriptFieldData Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template <typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 8, "The value size is too large");

			return *(T*)m_Buffer;
		}

		template <typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 8, "The value size is too large");
			memcpy(m_Buffer, &value, sizeof(T));
		}


	private :
		uint8_t m_Buffer[8];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};


	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& namespaceName, const std::string& className, bool isCore = false);
		MonoMethod* GetMethodByName(const std::string& methodName, int params);
		void Invoke(MonoMethod* method, MonoObject* instance, void** params);
		MonoObject* InstantiateClass();

		MonoClass* GetClass() const { return m_Class; }
		const std::map<std::string, ScriptFieldData>& GetFields();

	private:
		MonoClass* m_Class = nullptr;
		//MonoObject* m_ClassInstance = nullptr;

		std::string m_NameSpace;
		std::string m_Name;
		std::map<std::string, ScriptFieldData> m_ScriptFieldDataMap;

		void** m_Params = nullptr;

		friend class ScriptEngine;
		friend class ScriptInstance;
	};
	

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass>& scriptClass, Entity entity);
		void InvokeOnCreate();
		void InvokeOnUpdate(TimeStep ts);

		const Ref<ScriptClass>& GetScriptClass() const ;


		template <typename T>
		T& GetFieldValue(const std::string& name)
		{
			T value;

			GetValueInternally(name, &value);

			return value;
		}

		template <typename T>
		void SetFieldValue(const std::string& name, T& value)
		{
			SetValueInternally(name, &value);
		}

	private :

		void GetValueInternally(const std::string& name, void* val);
		void SetValueInternally(const std::string& name, const void* val);

	private:
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		Ref<ScriptClass> m_SClass;

		friend class ScriptEngine;
	};



	class ScriptEngine
	{

	public:

		static void Init();
		static void Shutdown();

		static void  LoadCoreAssembly(const std::filesystem::path& path);
		static void  LoadAppAssembly(const std::filesystem::path& path);

		static void OnRunTimeStart(Scene* scene);
		static void OnRunTimeStop();
		static void OnCreate(Entity entity);
		static void OnUpdate(Entity entity, TimeStep ts);
		static Scene* GetActiveContext();
		static MonoImage* GetMonoImage();

		static const Ref<ScriptInstance>& GetInstance(Entity entity);

		static ScriptFieldMap& GetEntityFieldMap(Entity entity);

		static  Ref<ScriptClass> GetEntityClass(const std::string& className);

	private:
		static MonoObject* Instantiate(MonoClass* className);
		static void InitMono();
		static void ShutdownMono();
		static MonoImage* GetImageAssy(MonoAssembly* coreAssy);

		static void LoadAssymblyClasses();

		friend class ScriptClass;

	};
}