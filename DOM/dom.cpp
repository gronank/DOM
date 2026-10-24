#include "dom.h"
#include <map>
#include <windows.h> 
#include <span>
#include <algorithm>

type* type_of_pointer(type* base_type, void* ptr)
{
	return base_type->type_of ? base_type->type_of(ptr) : base_type;
}
std::map<ce_module*, HINSTANCE> loadedModules;

ce_module* load_module(const char* path)
{
	HINSTANCE libPtr = LoadLibraryA(path);
	auto getModuleFunc = (load_module_func)GetProcAddress(libPtr, "load_module");
	ce_module* m = getModuleFunc?getModuleFunc():nullptr;
	if (!m)
	{
		FreeLibrary(libPtr);
		return nullptr;
	}

	loadedModules[m] = libPtr;
	return m;
}
DOM_EXPORT type* module_get_type(ce_module* mod, const char* name)
{
	auto types = std::span{ mod->types.begin, (size_t)mod->types.size };
	auto it = std::ranges::find_if(types, [&](const type& t) {return !strcmp(t.name, name);});
	if (it == types.end())
		return nullptr;
	return &*it;
}
instance type_create(type* typ)
{
	if (!typ->constructor)
		return {};
	return { .object = {.pointer = typ->constructor()}, .value_kind = api_type, .type = typ };
}

void instance_inc_ref(instance* ins)
{
	if(ins->value_kind == api_type && ins->type->inc_ref)
		ins->type->inc_ref(ins->object.pointer);
}

void instance_dec_ref(instance* ins)
{
	if(ins->type && ins->type->dec_ref)
		ins->type->dec_ref(ins->object.pointer);
}
property* instance_get_property_by_name_and_type(instance* ins, type* typ, const char* name)
{
	auto properties = std::span{ ins->type->properties.begin, (size_t)ins->type->properties.size };
	auto it = std::ranges::find_if(properties, [&](const property& p) {return !strcmp(p.name, name);});
	if (it != properties.end())
		return &*it;
	
	if (typ->parent)
		instance_get_property_by_name_and_type(ins, typ->parent, name);

	return nullptr;
}

property* instance_get_property_by_name(instance* ins, const char* name)
{
	if (ins->value_kind != api_type)
		return nullptr;
	
	return instance_get_property_by_name_and_type(ins, ins->type, name);
}

instance instance_get_property_value(instance* ins, property* prop)
{
	instance result;
	switch (prop->value_kind)
	{
	case api_type:
		result = { {.pointer = prop->getter.TYPE(ins->object.pointer)}, api_type };
		result.type = type_of_pointer(prop->type, result.object.pointer);
		instance_inc_ref(&result);
		break;
	case string:
		result = { {.string = prop->getter.STRING(ins->object.pointer)}, string };
		break;
	case integer:
		result = { {.integer = prop->getter.INTEGER(ins->object.pointer)}, integer };
	}
	
	return result;
}

void instance_set_property_value(instance* ins, property* prop, instance* value)
{
	if (prop->value_kind != value->value_kind || ins->value_kind != api_type)
		return;
	switch (prop->value_kind)
	{
	case api_type:
		prop->setter.TYPE(ins->object.pointer, value->object.pointer);
		break;
	case string:
		prop->setter.STRING(ins->object.pointer, value->object.string);
		break;
	case integer:
		prop->setter.INTEGER(ins->object.pointer, value->object.integer);
		break;
	}
}

int instance_list_size(instance* ins)
{
	auto* li = ins->type->list_info;
	if (!li)
		return 0;
	return li->list_size(ins->object.pointer);
}

instance instance_list_at(instance* ins, int index)
{
	auto* li = ins->type->list_info;
	if (!li)
		return {};
	instance result;
	switch (li->value_kind)
	{
	case api_type:
		result = { {.pointer = li->list_at.TYPE(ins->object.pointer, index)}, api_type };
		result.type = type_of_pointer(li->list_type, result.object.pointer);
		instance_inc_ref(&result);
		break;
	case string:
		result = { {.string = li->list_at.STRING(ins->object.pointer, index)}, string };
		break;
	case integer:
		result = { {.integer = li->list_at.INTEGER(ins->object.pointer, index)}, integer };
		break;
	}
	return result;
}

void instance_list_insert(instance* ins, int index, instance* value)
{
	auto* li = ins->type->list_info;
	if (!li || li->value_kind != value->value_kind)
		return;
	switch (li->value_kind)
	{
	case api_type:
		li->list_insert.TYPE(ins->object.pointer, index, value->object.pointer);
		break;
	case string:
		li->list_insert.STRING(ins->object.pointer, index, value->object.string);
		break;
	case integer:
		li->list_insert.INTEGER(ins->object.pointer, index, value->object.integer);
		break;
	}
}

void instance_list_remove(instance* ins, int index)
{
	if (!ins->type->list_info || ins->value_kind != api_type)
		return;
	ins->type->list_info->list_remove(ins->object.pointer, index);
}

void instance_list_add(instance* ins, instance* value)
{
	auto* li = ins->type->list_info;
	if (!li || li->value_kind != value->value_kind)
		return;
	switch (li->value_kind)
	{
	case api_type:
		li->list_add.TYPE(ins->object.pointer, value->object.pointer);
		break;
	case string:
		li->list_add.STRING(ins->object.pointer, value->object.string);
		break;
	case integer:
		li->list_add.INTEGER(ins->object.pointer, value->object.integer);
		break;
	}
}

DOM_EXPORT instance instance_string(const char* str)
{
	return { .object = {.string = str}, .value_kind = string };
}

DOM_EXPORT instance instance_integer(int i)
{
	return { .object = {.integer = i}, .value_kind = integer };
}
