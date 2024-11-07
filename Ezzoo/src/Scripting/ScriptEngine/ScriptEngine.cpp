#include "ezpch.h"
#include "ScriptEngine.h"
#include "Ezzoo/Core/Core.h"
#include "ScriptGlue.h"

#include "glm/glm.hpp"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/attrdefs.h"


#include "Scene/Component.h"

namespace Ezzoo {

	/*	enum ScriptFields
	{
		None = 0,
		Byte, Float, Int, Double, Long,
		UByte, UInt, ULong,
		Vector2, Vector3, Vector4
	};*/
	static std::unordered_map<std::string, ScriptFields> s_Fields =
	{
		{"System.Byte", ScriptFields::UByte},
		{"System.SByte", ScriptFields::Byte},

		{"System.Single", ScriptFields::Float},
		{"System.Bool", ScriptFields::Bool},
		{"System.Double", ScriptFields::Double},

		{"System.Int32", ScriptFields::Int},
		{"System.UInt32", ScriptFields::UInt},

		{"System.Int64", ScriptFields::Long},
		{"System.UInt64", ScriptFields::ULong},

		{"Ezzoo.Vector2", ScriptFields::Vector2},
		{"Ezzoo.Vector3", ScriptFields::Vector3},
		{"Ezzoo.Vector4", ScriptFields::Vector4},

		{"Ezzoo.Entity", ScriptFields::Entity},

	};


	struct MonoData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssy = nullptr;
		MonoImage* CoreAssyImage = nullptr;


		MonoAssembly* AppAssy = nullptr;
		MonoImage* AppAssyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		
		//store all class attribute in Edit time before invoking Create in run Time
		//Like Player class in C# is Entity and Has Attributes
		//Also to control the Attributes value in Editing time 
		std::unordered_map<UUID, ScriptFieldMap> EntityFieldMap;

		Scene* ActiveScene = nullptr;
	};


	static MonoData* s_MonoData;

	namespace Utils {


		static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			EZZOO_CORE_ASSERT(!(status != MONO_IMAGE_OK), mono_image_strerror(status));

			s_MonoData->CoreAssy = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return s_MonoData->CoreAssy;
		}

	

		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				EZZOO_CORE_TRACE("{0}.{1}", nameSpace, name);
			}
		}

		static const ScriptFields& MonoTypeToScriptFieldType(MonoType* type)
		{
			std::string name = mono_type_get_name(type);
			auto it = s_Fields.find(name);

			if (it == s_Fields.end())
				return ScriptFields::None;


			return it->second;

		}

		static const char*  ScriptFieldTypeToString (ScriptFields field)
		{
	
			switch (field)
			{
				case ScriptFields::Float:		return "Float";
				case ScriptFields::Bool:		return "Bool";
				case ScriptFields::Vector2:		return "Vector2";
				case ScriptFields::Vector3:		return "Vector3";
				case ScriptFields::Vector4:		return "Vector4";
				case ScriptFields::Byte:		return "Byte";
				case ScriptFields::UByte:		return "UByte";
				case ScriptFields::Long:		return "Long";
				case ScriptFields::ULong:		return "ULong";
				case ScriptFields::Int:			return "Int";
				case ScriptFields::UInt:		return "UIint";
				case ScriptFields::Double:		return "Double";
				case ScriptFields::Entity:		return "Entity";
			}

			return "<Invalid>";
		}

	}


	void ScriptEngine::Init()
	{
		s_MonoData = new MonoData;

		InitMono();
		LoadCoreAssembly("Scripts/Resources/Scripts/Ezzoo-ScriptCore.dll");
		LoadAppAssembly("Scripts/Resources/Scripts/Sandbox.dll");
		LoadAssymblyClasses();

		//Instantiate Class and Call Constructor
		s_MonoData->EntityClass = ScriptClass("Ezzoo", "Entity", true);

		ScriptGlue::InternalCalls();
		ScriptGlue::RegisterComponents();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_MonoData;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("EzzooJitRunTime");
		
		// Maybe log some error here
		EZZOO_CORE_ASSERT(!(rootDomain == nullptr), "Root Domain is null!!!!");

		// Store the root domain pointer
		s_MonoData->RootDomain = rootDomain;

		s_MonoData->AppDomain = mono_domain_create_appdomain("EzzooScriptRunTime", nullptr);
		mono_domain_set(s_MonoData->AppDomain, true);


#if 0
		ScriptGlue::InternalCalls();
		//PrintAssemblyTypes(s_MonoData->CoreAssy);

		//Instantiate Class and Call Constructor

		s_MonoData->EntityClass = ScriptClass("Ezzoo", "Entity");

		MonoObject* instance = s_MonoData->EntityClass.InstantiateClass(s_MonoData->EntityClass.GetClass());

		//Call Method 
		MonoMethod* floatMethod = s_MonoData->EntityClass.GetMethodByName("PrintFloatVar", 0);
		EZZOO_CORE_ASSERT(floatMethod, "Method Does not Exist!!!");
		s_MonoData->EntityClass.Invoke(floatMethod, instance, nullptr);

		

		//Print int Function
		MonoMethod* IntMethod = s_MonoData->EntityClass.GetMethodByName("PrintIntVar", 2);
		EZZOO_CORE_ASSERT(IntMethod, "Method Does not Exist!!!");

		int value1 = 5;
		int value2 = 2;
		void* params[2]
		{
			&value1,
			&value2
		};
		s_MonoData->EntityClass.Invoke(IntMethod, instance, params);

		//Invoke String Param to Function 
		MonoString* string = mono_string_new(s_MonoData->AppDomain, "Hello From C++!!!!");
		MonoMethod* strMethod = s_MonoData->EntityClass.GetMethodByName("PrintString", 1);
		void* str = string;
		s_MonoData->EntityClass.Invoke(strMethod, instance, &str);


#endif
		//EZZOO_CORE_ASSERT(false, "Stop Here!!!!");
	}
	void ScriptEngine::LoadAssymblyClasses()
	{

		s_MonoData->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_MonoData->AppAssyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_MonoData->CoreAssyImage, "Ezzoo", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_MonoData->AppAssyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_MonoData->AppAssyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* className = mono_class_from_name(s_MonoData->AppAssyImage, nameSpace, name);

			if (className == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(className, entityClass, false);

			if (!isEntity)
				continue;
			
			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, name);
			s_MonoData->EntityClasses[fullName] = scriptClass;

			void* fields = nullptr;
			while (MonoClassField* field = mono_class_get_fields(className, &fields))
			{
				uint32_t flag = mono_field_get_flags(field);
				if (flag & MONO_FIELD_ATTR_PUBLIC)
				{
					const char* name = mono_field_get_name(field);

					ScriptFields type = Utils::MonoTypeToScriptFieldType(mono_field_get_type(field));
					const char* TypeName = Utils::ScriptFieldTypeToString(type);

					EZZOO_CORE_WARNING("{} - {}", name, TypeName);
					scriptClass->m_ScriptFieldDataMap[name] = { name, type, field };
				}
			}
		}
	}
	
	
	void ScriptEngine::LoadCoreAssembly(const std::filesystem::path& path)
	{
		s_MonoData->CoreAssy =  Utils::LoadCSharpAssembly(path.string());
		s_MonoData->CoreAssyImage = GetImageAssy(s_MonoData->CoreAssy);
	}



	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& path)
	{
		s_MonoData->AppAssy = Utils::LoadCSharpAssembly(path.string());
		s_MonoData->AppAssyImage = GetImageAssy(s_MonoData->AppAssy);
	}

	void ScriptEngine::OnRunTimeStart(Scene* scene)
	{
		s_MonoData->ActiveScene = scene;
	}

	void ScriptEngine::OnRunTimeStop()
	{
		s_MonoData->ActiveScene = nullptr;
	}

	void ScriptEngine::OnCreate(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		const auto& idC = entity.GetComponent<IDComponent>();
		UUID id = idC.ID;

		//TODO[Ezzoo] : MonoClass* to call Instatntiation of the c# class and pass to new ScriptInstance class;

		if (s_MonoData->EntityClasses[sc.ClassName] != nullptr)
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_MonoData->EntityClasses[sc.ClassName], entity);
			s_MonoData->EntityInstances[id] = instance;

			if (s_MonoData->EntityFieldMap.find(id) != s_MonoData->EntityFieldMap.end())
			{
				const ScriptFieldMap& fieldMap = s_MonoData->EntityFieldMap.at(id);
				for (const auto& [name, fieldInstance] : fieldMap)
					instance->SetValueInternally(name, (void*)fieldInstance.m_Buffer);
			}


			instance->InvokeOnCreate();
		}



 	}

	void ScriptEngine::OnUpdate(Entity entity, TimeStep ts)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		const auto& idC = entity.GetComponent<IDComponent>();
		//TODO[Ezzoo] : MonoClass* to call Instatntiation of the c# class and pass to new ScriptInstance class;

		if (s_MonoData->EntityClasses[sc.ClassName] != nullptr)
		{
			 s_MonoData->EntityInstances[idC.ID]->InvokeOnUpdate(ts);
		}
	}

	Scene* ScriptEngine::GetActiveContext()
	{
		return s_MonoData->ActiveScene;
	}

	MonoImage* ScriptEngine::GetMonoImage()
	{
		return s_MonoData->CoreAssyImage;
	}

	

	const Ref<ScriptInstance>& ScriptEngine::GetInstance(Entity entity)
	{
		const auto& idc = entity.GetComponent<IDComponent>();

		auto it = s_MonoData->EntityInstances.find(idc.ID);
		if (it == s_MonoData->EntityInstances.end())
			return nullptr;

		return s_MonoData->EntityInstances[idc.ID];
	}

	void ScriptEngine::ShutdownMono()
	{

		s_MonoData->AppDomain = nullptr;
		s_MonoData->CoreAssy = nullptr;
		s_MonoData->AppAssy = nullptr;
		s_MonoData->RootDomain = nullptr;
	}

	MonoImage* ScriptEngine::GetImageAssy(MonoAssembly* coreAssy)
	{
		return mono_assembly_get_image(coreAssy);
	}

	
	MonoObject* ScriptEngine::Instantiate(MonoClass* className)
	{
		MonoObject* instance = mono_object_new(s_MonoData->AppDomain, className);
		mono_runtime_object_init(instance);
		return instance;
	}


	ScriptFieldMap& ScriptEngine::GetEntityFieldMap(Entity entity)
	{
		const auto& idC = entity.GetComponent<IDComponent>();
		return s_MonoData->EntityFieldMap[idC.ID];

	}

	 Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& className)
	{
		auto it = s_MonoData->EntityClasses.find(className);
		if (it == s_MonoData->EntityClasses.end())
			return nullptr;

		return s_MonoData->EntityClasses[className];
	}

	//////////////////////////////////////////////////////////////////
	ScriptClass::ScriptClass(const std::string& namespaceName, const std::string& className, bool isCore)
		:m_NameSpace(namespaceName), m_Name(className)
	{
		m_Class = mono_class_from_name(isCore ? s_MonoData->CoreAssyImage : s_MonoData->AppAssyImage, namespaceName.c_str(), className.c_str());
	}

	MonoMethod* ScriptClass::GetMethodByName(const std::string& methodName, int params)
	{
		return mono_class_get_method_from_name(m_Class, methodName.c_str(), params);
	}
	
	void ScriptClass::Invoke(MonoMethod* method, MonoObject* instance, void** params)
	{
		mono_runtime_invoke(method, instance, params, nullptr);
	}

	MonoObject* ScriptClass::InstantiateClass()
	{
		return ScriptEngine::Instantiate(m_Class);
	}
	const std::map<std::string, ScriptFieldData>& ScriptClass::GetFields()
	{
		return m_ScriptFieldDataMap;
	}


	///////////////////////////////////////////////////////////////
	//ScriptInstance

	ScriptInstance::ScriptInstance(Ref<ScriptClass>& scriptClass, Entity entity)
		: m_SClass(scriptClass)
	{
		m_Instance = scriptClass->InstantiateClass();

		m_Constructor = s_MonoData->EntityClass.GetMethodByName(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethodByName("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethodByName("OnUpdate", 1);

		{
			const auto& idC = entity.GetComponent<IDComponent>();
			uint64_t id = idC.ID;
			void* param = &id;

			m_SClass->Invoke(m_Constructor, m_Instance, &param);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		//MonoMethod* method = m_SClass->GetMethodByName("OnCreate", 0);

		if (m_OnCreateMethod)
			m_SClass->Invoke(m_OnCreateMethod, m_Instance, nullptr);
	}

	void ScriptInstance::InvokeOnUpdate(TimeStep ts)
	{
		//MonoMethod* method = m_SClass->GetMethodByName("OnUpdate", 1);
		if (m_OnUpdateMethod)
		{
			float value = (float)ts;
			void* param = &value;

			m_SClass->Invoke(m_OnUpdateMethod, m_Instance, &param);

		}
	}

	const Ref<ScriptClass>& ScriptInstance::GetScriptClass() const 
	{
		return m_SClass;
	}

	void ScriptInstance::GetValueInternally(const std::string& name, void* val)
	{
		MonoClassField* field = m_SClass->m_ScriptFieldDataMap[name].ClassField;

		mono_field_get_value(m_Instance, field, val);
	}

	void ScriptInstance::SetValueInternally(const std::string& name, const void* val)
	{
		MonoClassField* field = m_SClass->m_ScriptFieldDataMap[name].ClassField;

		mono_field_set_value(m_Instance, field, (void*)val);
	}


}