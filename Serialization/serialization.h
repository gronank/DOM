#include "serialization_export.h"
#include <dom_interface.h>

extern "C"
{
	typedef struct configuration configuration;

	SERIALIZATION_EXPORT configuration* configuration_create();
	SERIALIZATION_EXPORT void configuration_free(configuration*);

	SERIALIZATION_EXPORT int configuration_instance_size(configuration*);
	SERIALIZATION_EXPORT instance configuration_instance_at(configuration*, int);
	SERIALIZATION_EXPORT void configuration_add_instance(configuration*, instance*);

	SERIALIZATION_EXPORT void configuration_serialize(configuration*, const char*);
	SERIALIZATION_EXPORT configuration* configuration_deserialize(const char*);
	
}