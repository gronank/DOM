#ifndef DOM_DEFINED
#define DOM_DEFINED
#include <dom_interface.h>
#include "samplelibrary_export.h"
extern "C"
{
	SAMPLELIBRARY_EXPORT ce_module* load_module();
	SAMPLELIBRARY_EXPORT type* view_type();
}
#endif
