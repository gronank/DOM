#ifndef DOM_DEFINE
#define DOM_DEFINE
#include "dom_interface.h"
#include "dom_export.h"

extern "C"
{
	//DOM_EXPORT ce_module* load_module(const char* path);
	//DOM_EXPORT type* module_get_type(ce_module*, const char*);
	DOM_EXPORT instance type_create(type*);
	DOM_EXPORT void instance_inc_ref(instance*);
	DOM_EXPORT void instance_dec_ref(instance*);

	DOM_EXPORT property* instance_get_property_by_name(instance*, const char*);
	DOM_EXPORT instance instance_get_property_value(instance*, property*);
	DOM_EXPORT void instance_set_property_value(instance*, property*, instance*);

	DOM_EXPORT int instance_list_size(instance*);
	DOM_EXPORT instance instance_list_at(instance*, int);
	DOM_EXPORT void instance_list_insert(instance*, int, instance*);
	DOM_EXPORT void instance_list_remove(instance*, int);
	DOM_EXPORT void instance_list_add(instance*, instance*);

	DOM_EXPORT instance instance_string(const char*);
	DOM_EXPORT instance instance_integer(int);
}
#endif DOM_DEFINE