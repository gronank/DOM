#ifndef DOM_INTERFACE_DEFINE
#define DOM_INTERFACE_DEFINE
extern "C"
{
	// Properties
	typedef struct type type;
	typedef struct instance instance;
	typedef struct ce_module ce_module;
	typedef void* (*set_property_t)(void*, void*);
	typedef void* (*set_property_s)(void*, const char*);
	typedef void* (*set_property_i)(void*, int);
	typedef void* (*get_property_t)(void*);
	typedef const char* (*get_property_s)(void*);
	typedef int (*get_property_i)(void*);
	typedef void (*inc_ref)(void*);
	typedef void (*dec_ref)(void*);
	typedef type* (*type_of)(void*);
	typedef instance* (*constructor)();
	typedef int (*list_size)(void*);
	typedef void* (*list_at_t)(void*, int);
	typedef const char* (*list_at_s)(void*, int);
	typedef int (*list_at_i)(void*, int);
	typedef void (*list_remove)(void*, int);
	typedef void (*list_insert_t)(void*, int, void*);
	typedef void (*list_insert_s)(void*, int, const char*);
	typedef void (*list_insert_i)(void*, int, int);
	typedef void (*list_add_t)(void*, void*);
	typedef void (*list_add_s)(void*, const char*);
	typedef void (*list_add_i)(void*, int);
	typedef ce_module* (*load_module_func)();
	
	typedef enum value_kind
	{
		api_type,
		string,
		integer
	} value_kind;

	typedef struct property
	{
		const char* name;
		type* type;
		value_kind value_kind;
		union 
		{
			get_property_t TYPE;
			get_property_s STRING;
			get_property_i INTEGER;
		} getter;
		union
		{
			set_property_t TYPE;
			set_property_s STRING;
			set_property_i INTEGER;
		} setter;
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
		value_kind value_kind;
		list_size list_size;

		union {
			list_at_t TYPE;
			list_at_s STRING;
			list_at_i INTEGER;
		} list_at;

		list_remove list_remove;

		union {
			list_insert_t TYPE;
			list_insert_s STRING;
			list_insert_i INTEGER;
		} list_insert;

		union {
			list_add_t TYPE;
			list_add_s STRING;
			list_add_i INTEGER;
		} list_add;
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
		union {
			void* pointer;
			const char* string;
			int integer;
		} object;
		value_kind value_kind;
		type* type;
	} instance;


	//module* load_module(const char*);
}
#endif DOM_INTERFACE_DEFINE