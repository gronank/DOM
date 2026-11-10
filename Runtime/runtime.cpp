#include "runtime.h"
#include <memory>
#include <map>
#include <string>
#include <windows.h> 
#include <span>
#include <cassert>

struct LoadedModule
{
	~LoadedModule()
	{
		FreeLibrary(libPtr);
		// module owned by lib
	}

	HINSTANCE libPtr;
	ce_module* module;
};

type string_t
{
	.name = "String"
};

type integer_t
{
	.name = "Integer"
};

struct runtime
{
	std::map<std::string, LoadedModule> loadedModules;
	std::map<std::string, type*> loadedTypes{ {"String",&string_t},{"Integer", &integer_t} };
} default_runtime;


bool initialize_module(const char* path)
{
	auto it = default_runtime.loadedModules.find(path);
	if (it != default_runtime.loadedModules.end())
		return true;

	HINSTANCE libPtr = LoadLibraryA(path);
	auto getModuleFunc = (load_module_func)GetProcAddress(libPtr, "load_module");
	ce_module* m = getModuleFunc ? getModuleFunc(&default_runtime) : nullptr;
	if (!m)
	{
		FreeLibrary(libPtr);
		return false;
	}
	else
	{
		default_runtime.loadedModules[path] = { libPtr, m };
		
		for (type& t : std::span{ m->types.begin, (size_t)m->types.size })
		{
			assert(!default_runtime.loadedTypes.contains(t.name));
			default_runtime.loadedTypes[t.name] = &t;
		}
		return true;
	}
}

ce_module* get_module(const char* path)
{
	initialize_module(path);
	auto it = default_runtime.loadedModules.find(path);
	if (it == default_runtime.loadedModules.end())
		return nullptr;
	return it->second.module;
}

type* get_type(const char* name)
{
	auto it = default_runtime.loadedTypes.find(name);
	if (it == default_runtime.loadedTypes.end())
	{
		assert(false);
		return nullptr;
	}
	return it->second;
}

type* type_of_object(type* base_type, object_id* ptr)
{
	return base_type->type_of ? get_type(base_type->type_of(*ptr)) : base_type;
}

