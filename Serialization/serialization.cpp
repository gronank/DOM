#include "serialization.h"
#include <dom.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <span>
#include <fstream>

struct InstanceWrapper
{
	instance value;
	InstanceWrapper(instance* instance): value{*instance}
	{
		instance_inc_ref(&value);
	}
	InstanceWrapper(InstanceWrapper&& other) :value(other.value)
	{
		other.value = {};
	}
	~InstanceWrapper()
	{
		instance_dec_ref(&value);
	}
};

struct configuration
{
	std::vector<InstanceWrapper> instances;
};

configuration* configuration_create()
{
	return new configuration();
}

void configuration_free(configuration* conf)
{
	delete conf;
}

int configuration_instance_size(configuration* conf)
{
	return (int)conf->instances.size();
}

instance configuration_instance_at(configuration* conf, int index)
{
	auto result = conf->instances[index].value;
	instance_inc_ref(&result);
	return result;
}

void configuration_add_instance(configuration* conf, instance* ins)
{
	conf->instances.emplace_back(ins);
}

namespace {
	using namespace nlohmann;

	json toJson(instance* ins)
	{
		if (ins->type->list_info)
		{
			auto listResult = json::array();
			int size = instance_list_size(ins);
			for (int i = 0; i < size; i++)
			{
				auto item = instance_list_at(ins, i);
				listResult.push_back(toJson(&item));
				instance_dec_ref(&item);
			}
			return listResult;
		}
		if (ins->type == &string_t)
			return reinterpret_cast<const char*>(&ins->object);
		else if (ins->type == &integer_t)
			return *reinterpret_cast<int*>(&ins->object);
		else
			return json::object({ {"id", (long long)*reinterpret_cast<void**>(&ins->object)} });
	}

	void collectInstances(std::map<void*, json>& uniqueInstances, instance* ins)
	{
		if (ins->type->list_info)
		{
			int size = instance_list_size(ins);
			for (int i = 0; i < size; i++)
			{
				auto item = instance_list_at(ins, i);
				collectInstances(uniqueInstances, &item);
			}
			return;
		}
		if (uniqueInstances.contains(&ins->object) || ins->type->module == nullptr)
			return;
		auto result = json::object();
		auto objectId = *reinterpret_cast<void**>(&ins->object);
		result["type"] = ins->type->name;
		result["id"] = (long long)objectId;
		assert(!ins->type->list_info);

		auto propertyValues = json::object();
		for (auto* type = ins->type;type; type = type->parent)
		{
			auto properties = std::span{ type->properties.begin, (size_t)type->properties.size };
			for (auto& property : properties)
			{
				auto propertyValue = instance_get_property_value(ins, &property);
				collectInstances(uniqueInstances, &propertyValue);
				propertyValues[property.name] = toJson(&propertyValue);
				instance_dec_ref(&propertyValue);
			}
		}
		result["properties"] = propertyValues;
		result["module"] = ins->type->module->name;
		uniqueInstances[objectId] = result;
	}


}

void configuration_serialize(configuration* config, const char* path)
{
	std::map<void*, json> uniqueInstances;
	
	for (auto& instance : config->instances)
	{
		collectInstances(uniqueInstances, &instance.value);
	}
	auto result = json::array();
	for (const auto& [_, object] : uniqueInstances)
	{
		result.push_back(object);
	}
	std::ofstream o(path);
	o << std::setw(4) << result;
}
instance createInstance(ce_module* module, const json& object)
{
	std::string typeName = object["type"];
	auto type = module_get_type(module, typeName.c_str());
	return type_create(type);
}
instance parseValue(std::map<long long, instance>& loadedInstances, const json& object)
{
	if (object.is_string())
	{
		// This must be static so that the value lives until it is inserted (and copied) into the value
		static thread_local std::string itemString = object; 
		return instance_string(itemString.c_str());
	}
	else if (object.is_object())
	{
		return loadedInstances[object["id"]];
	}
	else
	{
		assert(object.is_number_integer());
		return instance_integer(object);
	}
}

void assignProperties(std::map<long long, instance>& loadedInstances, const json& object)
{
	auto& objectInstance = loadedInstances[object["id"]];
	for (const auto& [propertyName, propertyValue]: object["properties"].items())
	{
		auto property = instance_get_property_by_name(&objectInstance, propertyName.c_str());
		if ( property->type->list_info)
		{
			auto listInstance = instance_get_property_value(&objectInstance, property);
			for (const auto& item : propertyValue)
			{
				instance itemInstance = parseValue(loadedInstances, item);
				instance_list_add(&listInstance, &itemInstance);
			}
		}
		else
		{
			instance propertyInstance = parseValue(loadedInstances, propertyValue);
			instance_set_property_value(&objectInstance, property, &propertyInstance);
		}
	}
}

configuration* configuration_deserialize(const char* path)
{
	std::map<std::string, ce_module*> loadedModules;
	std::map<long long, instance> loadedInstances;

	std::ifstream in(path);
	json objects;
	in >> objects;
	// Create objects
	for (const auto& object : objects)
	{
		std::string moduleName = object["module"];
		auto& module = loadedModules[moduleName];
		if (!module)
			module = load_module(moduleName.c_str());
		loadedInstances[object["id"]] = createInstance(module, object);
	}

	// Populate properties
	for (const auto& object : objects)
	{
		assignProperties(loadedInstances, object);
	}

	// Put the configuration together
	auto* config = new configuration{};
	for (auto& [_, instance] : loadedInstances)
	{
		config->instances.emplace_back(&instance);
		instance_dec_ref(&instance);
	}
	
	return config;
}
