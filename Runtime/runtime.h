#ifndef RUNTIME_DEFINED
#define RUNTIME_DEFINED
#include "runtime_export.h"
#include <dom_interface.h>
extern "C"
{
	typedef struct runtime runtime;
	typedef struct ce_module ce_module;
	bool initialize_module(const char*);
	ce_module* get_module(const char*);
	type* get_type(const char*);
	type* type_of_object(type*, object_id*);
		
	typedef ce_module* (*load_module_func)(runtime*);
}
#endif // !RUNTIME_DEFINED
