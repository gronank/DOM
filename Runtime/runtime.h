#ifndef RUNTIME_DEFINED
#define RUNTIME_DEFINED
#include "runtime_export.h"
#include <dom_interface.h>
extern "C"
{
	typedef struct runtime runtime;
	typedef struct ce_module ce_module;
	RUNTIME_EXPORT bool initialize_module(const char*);
	RUNTIME_EXPORT ce_module* get_module(const char*);
	RUNTIME_EXPORT type* get_type(const char*);
	RUNTIME_EXPORT type* type_of_object(type*, object_id*);
		
	typedef ce_module* (*load_module_func)();
}
#endif // !RUNTIME_DEFINED
