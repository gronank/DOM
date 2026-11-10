#ifndef DOM_INTERFACE_DEFINE
#define DOM_INTERFACE_DEFINE
extern "C"
{
	// Properties
	typedef struct type type;
	typedef struct instance instance;
	typedef void* object_id;
	typedef struct ce_module ce_module;
	typedef void (*set_property)(object_id, object_id);
	typedef object_id (*get_property)(object_id);
	typedef void (*inc_ref)(object_id);
	typedef void (*dec_ref)(object_id);
	typedef type* (*type_of)(object_id);
	typedef object_id (*constructor)();
	typedef int (*list_size)(object_id);
	typedef object_id (*list_at)(object_id, int);
	typedef void (*list_remove)(object_id, int);
	typedef void (*list_insert)(object_id, int, object_id);
	typedef void (*list_add)(object_id, object_id);
	typedef ce_module* (*load_module_func)();

	typedef struct property
	{
		const char* name;
		type* type;
		get_property get;
		set_property set;
	} property;

	typedef struct property_list
	{
		property* begin;
		int size;
	} property_list;

	//// Static property
	//typedef struct type type;
	//typedef void (*set_static_property_t)(type*, instance*);
	//typedef void (*set_static_property_s)(type*, const char*);
	//typedef void (*set_static_property_i)(type*, int);
	//typedef instance* (*get_static_property_t)(type*);
	//typedef const char* (*get_static_property_s)(type*);
	//typedef int (*get_static_property_i)(type*);
	//typedef void (*inc_ref)(void*);
	//typedef void (*dec_ref)(void*);

	//typedef struct static_property
	//{
	//} static_property;

	//typedef struct static_property_list
	//{
	//	static_property* begin;
	//	int size;
	//} static_property_list;

	//typedef struct static_property_t : public static_property
	//{
	//	set_static_property_t setter;
	//	get_static_property_t getter;
	//	instance* default_value;
	//} static_property_t;

	//typedef struct static_property_s : public static_property
	//{
	//	set_static_property_s setter;
	//	get_static_property_s getter;
	//	const char* default_value;
	//} static_property_s;

	//typedef struct static_property_i : public static_property
	//{
	//	set_static_property_i setter;
	//	get_static_property_i getter;
	//	int default_value;
	//} static_property_i;

	
	// List info
	typedef struct list_info
	{
		type* list_type;
		list_size size;
		list_at at;
		list_remove remove;
		list_insert insert;
		list_add add;
	} list_info;

	// Type
	typedef struct type
	{
		const char* name;
		type* parent;
		constructor constructor;
		property_list properties;
		//static_property_list static_properties;
		list_info* list_info;
		inc_ref inc_ref;
		dec_ref dec_ref;
		type_of type_of;
		ce_module* module;
	} type;

	typedef struct type_list
	{
		type* begin;
		int size;
	} type_list;

	// Module
	typedef struct ce_module
	{
		const char* name;
		type_list types;
	} ce_module;

	// Instance
	typedef struct instance
	{
		object_id object;
		type* type;
	} instance;

	extern type string_t;

	extern type integer_t;

	//module* load_module(const char*);
}
#endif DOM_INTERFACE_DEFINE