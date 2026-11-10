#include "dom.h"
#include <map>
#include <windows.h> 
#include <span>
#include <algorithm>
#include <runtime.h>


instance type_create(type* typ)
{
	if (!typ->constructor)
		return {};
	return { .object = typ->constructor(), .type = typ };
}

void instance_inc_ref(instance* ins)
{
	if(ins->type->inc_ref)
		ins->type->inc_ref(ins->object);
}

void instance_dec_ref(instance* ins)
{
	if(ins->type->dec_ref)
		ins->type->dec_ref(ins->object);
}
instance resolve_instance(object_id id, type* base_type)
{
	instance ins = { .object = id, .type = type_of_object(base_type, &id) };
	instance_inc_ref(&ins);
	return ins;
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
	return instance_get_property_by_name_and_type(ins, ins->type, name);
}

instance instance_get_property_value(instance* ins, property* prop)
{
	return resolve_instance(prop->get(ins->object), prop->type);
};

void instance_set_property_value(instance* ins, property* prop, instance* value)
{
	prop->set(ins->object, value->object);
}

int instance_list_size(instance* ins)
{
	auto* li = ins->type->list_info;
	if (!li)
		return 0;
	return li->size(ins->object);
}

instance instance_list_at(instance* ins, int index)
{
	auto* list = ins->type->list_info;
	if (!list)
		return {};
	return resolve_instance(list->at(ins->object, index), list->list_type);
}

void instance_list_insert(instance* ins, int index, instance* value)
{
	auto* list = ins->type->list_info;
	if (!list)
		return;
	list->insert(ins->object, index, value->object);
}

void instance_list_remove(instance* ins, int index)
{
	if (!ins->type->list_info)
		return;
	ins->type->list_info->remove(ins->object, index);
}

void instance_list_add(instance* ins, instance* value)
{
	auto* list = ins->type->list_info;
	if (!list)
		return;
	list->add(ins->object, value->object);
}

auto string_t = get_type("String");
auto integer_t = get_type("Integer");

DOM_EXPORT instance instance_string(const char* str)
{
	return { .object = reinterpret_cast<object_id>((char*)str), .type = string_t };
}

DOM_EXPORT instance instance_integer(int i)
{
	return { .object = *reinterpret_cast<object_id*>(&i), .type = integer_t };
}


