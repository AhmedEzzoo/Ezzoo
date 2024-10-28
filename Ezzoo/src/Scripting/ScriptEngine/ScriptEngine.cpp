#include "ezpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"


#include "Ezzoo/Core/Core.h"
namespace Ezzoo {


	struct MonoData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssy = nullptr;
	};


	static MonoData* s_Data;


	void ScriptEngine::Init()
	{
		s_Data = new MonoData;

		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}


	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		EZZOO_CORE_ASSERT(!(status != MONO_IMAGE_OK), mono_image_strerror(status));

		s_Data->CoreAssy = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return s_Data->CoreAssy;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
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

	MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		EZZOO_CORE_ASSERT(s_Data->CoreAssy, "Failed To Load Class!!");
		

		return klass;
	}


	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("EzzooJitRunTime");
		
		// Maybe log some error here
		EZZOO_CORE_ASSERT(!(rootDomain == nullptr), "Root Domain is null!!!!");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		s_Data->AppDomain = mono_domain_create_appdomain("EzzooScriptRunTime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssy = LoadCSharpAssembly("Resources/Scripts/Ezzoo-ScriptCore.dll");

		PrintAssemblyTypes(s_Data->CoreAssy);

		//Instantiate Class and Call Constructor
		MonoImage* image = mono_assembly_get_image(s_Data->CoreAssy);
		MonoClass* mClass = mono_class_from_name(image, "Ezzoo", "Main");
		MonoObject* instance = mono_object_new(s_Data->AppDomain, mClass);
		mono_runtime_object_init(instance);

		//Call Method 
		MonoMethod* floatMethod = mono_class_get_method_from_name(mClass, "PrintFloatVar", 0);
		EZZOO_CORE_ASSERT(floatMethod, "Method Does not Exist!!!");
		mono_runtime_invoke(floatMethod, instance, nullptr, nullptr);

		//Print int Function
		MonoMethod* IntMethod = mono_class_get_method_from_name(mClass, "PrintIntVar", 2);
		EZZOO_CORE_ASSERT(IntMethod, "Method Does not Exist!!!");

		int value1 = 5;
		int value2 = 2;
		void* params[2]
		{
			&value1,
			&value2
		};
		mono_runtime_invoke(IntMethod, instance, params, nullptr);

		//Invoke String Param to Function 
		MonoString* string = mono_string_new(s_Data->AppDomain, "Hello From C++!!!!");
		MonoMethod* strMethod = mono_class_get_method_from_name(mClass, "PrintString", 1);
		void* str = string;
		mono_runtime_invoke(strMethod, instance, &str, nullptr);



		EZZOO_CORE_ASSERT(false, "Stop Here!!!!");
	}



	void ScriptEngine::ShutdownMono()
	{

		s_Data->AppDomain = nullptr;
		s_Data->CoreAssy = nullptr;
		s_Data->RootDomain = nullptr;
	}



}