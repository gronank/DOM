#include <iostream>
#include <dom.h>
#include <span>
#include <vector>
#include <memory>
#include <serialization.h>
#include <iomanip>
void export_objects(ce_module* m)
{
	auto view_t = module_get_type(m, "View");
	auto v = type_create(view_t);

	auto layer_t = module_get_type(m, "Layer");
	auto layersProperty = instance_get_property_by_name(&v, "Layers");
	auto layerList = instance_get_property_value(&v, layersProperty);

	{
		auto l = type_create(layer_t);
		instance_list_add(&layerList, &l);

		auto stylesProperty = instance_get_property_by_name(&l, "Styles");
		auto stylesList = instance_get_property_value(&l, stylesProperty);

		{
			auto symbol_style_t = module_get_type(m, "SymbolStyle");
			auto symbolStyle = type_create(symbol_style_t);
			auto pathProperty = instance_get_property_by_name(&symbolStyle, "Path");
			auto path = instance_string("MyImage.png");
			instance_set_property_value(&symbolStyle, pathProperty, &path);

			instance_list_add(&stylesList, &symbolStyle);
			instance_dec_ref(&symbolStyle);
		}
		instance_dec_ref(&l);
	}
	{
		auto l = type_create(layer_t);
		instance_list_add(&layerList, &l);

		auto stylesProperty = instance_get_property_by_name(&l, "Styles");
		auto stylesList = instance_get_property_value(&l, stylesProperty);

		{
			auto symbol_style_t = module_get_type(m, "LineStyle");
			auto lineStyle = type_create(symbol_style_t);
			auto widthProperty = instance_get_property_by_name(&lineStyle, "Width");
			auto width = instance_integer(15);
			instance_set_property_value(&lineStyle, widthProperty, &width);

			instance_list_add(&stylesList, &lineStyle);
			instance_dec_ref(&lineStyle);
		}
		instance_dec_ref(&l);
	}

	auto c = std::unique_ptr<configuration, decltype(&configuration_free)>(configuration_create(), configuration_free);
	configuration_add_instance(c.get(), &v);
	configuration_serialize(c.get(), "myConfig.pj");
	instance_dec_ref(&v);
}
auto indent = std::string_view("                                          ");
void printInstance(instance* ins, int indentation)
{
	if (ins->type == &string_t)
		std::cout << *reinterpret_cast<const char*>(ins->object) << "\n";
	else if(ins->type == &integer_t)
		std::cout << *reinterpret_cast<int*>(&ins->object) << "\n";
	else if (ins->type->list_info)
	{
		std::cout << "[\n";
		auto size = instance_list_size(ins);
		for (int i = 0;i < size;i++)
		{
			auto item = instance_list_at(ins, i);
			std::cout << indent.substr(0,2 * indentation + 2);
			printInstance(&item, indentation + 1);
			instance_dec_ref(&item);
		}
		std::cout << indent.substr(0,2 * indentation)<<"]\n";
	}
	else
	{
		std::cout << ins->type->name << "\n";
		auto properties = std::span{ ins->type->properties.begin, (size_t)ins->type->properties.size };
		for (auto& property : properties)
		{
			std::cout << indent.substr(0,2 * indentation + 2) << property.name << ": ";
			auto value = instance_get_property_value(ins, &property);
			printInstance(&value, indentation + 1);
			instance_dec_ref(&value);
		}
	}
	
}

void import_objects(const char* path)
{
	auto config = configuration_deserialize(path);
	auto size = configuration_instance_size(config);
	instance myView;
	for (int i = 0;i < size;i++)
	{
		auto ins = configuration_instance_at(config, i);
		if (!strcmp(ins.type->name, "View"))
		{
			myView = ins;
			break;
		}
		instance_dec_ref(&ins);
	}
	printInstance(&myView, 0);
	instance_dec_ref(&myView);
	configuration_free(config);
}
void printTypes(ce_module* mod)
{
	std::cout << "Module loaded: " << mod->name << "\n";
	auto types = std::span{ mod->types.begin, (size_t)mod->types.size };
	for (const auto& type : types)
	{
		std::string base = "";
		if (type.parent)
			base = std::string(" (") + type.parent->name + ")";
		std::cout << "  " << type.name << base << "\n";

		auto properties = std::span{ type.properties.begin, (size_t)type.properties.size };
		if (properties.empty())
			continue;

		for (const auto& property : properties)
		{

			auto propertyTypeName =
				property.type == &string_t ? "String" :
				property.type == &integer_t ? "Integer" :
				property.type->name;
			std::string access = "[get";
			if (property.set)
				access += ", set";
			access += "]";
			std::cout << "    " << property.name << " {" << propertyTypeName << "} " << access << "\n";
		}
	}
}
int main()
{
	auto mod = load_module("SampleLibrary");
	if (!mod)
	{
		std::cout << "Failed to load module: SampleLibrary\n";
		return 0;
	}
	printTypes(mod);
	std::cout << "\nExporting constructed config...\n";
	export_objects(mod);
	std::cout << "\n";
	std::cout << "Loaded config:\n";
	import_objects("myConfig.pj");
}