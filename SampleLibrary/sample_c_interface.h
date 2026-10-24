#ifndef C_INTERFACE_DEFINE
#define C_INTERFACE_DEFINE
#include "samplelibrary_export.h"
extern  "C"
{
	SAMPLELIBRARY_EXPORT typedef struct view view;
	SAMPLELIBRARY_EXPORT typedef struct layer layer;
	SAMPLELIBRARY_EXPORT typedef struct style {} style;
	SAMPLELIBRARY_EXPORT typedef struct symbol_style : style {} symbol_style;
	SAMPLELIBRARY_EXPORT typedef struct line_style : style {} line_style;
	SAMPLELIBRARY_EXPORT typedef struct layer_list layer_list;
	SAMPLELIBRARY_EXPORT typedef struct style_list style_list;

	// View
	SAMPLELIBRARY_EXPORT view* create_view();
	SAMPLELIBRARY_EXPORT void decref_view(view*);
	SAMPLELIBRARY_EXPORT void incref_view(view*);
	SAMPLELIBRARY_EXPORT layer_list* view_get_layers(view*);

	// Layer list
	SAMPLELIBRARY_EXPORT int layer_list_get_size(layer_list*);
	SAMPLELIBRARY_EXPORT layer* layer_list_at(layer_list*, int);
	SAMPLELIBRARY_EXPORT void layer_list_remove(layer_list*, int);
	SAMPLELIBRARY_EXPORT void layer_list_insert(layer_list*, int, layer*);
	SAMPLELIBRARY_EXPORT void layer_list_add(layer_list*, layer*);

	// Layer
	SAMPLELIBRARY_EXPORT layer* create_layer();
	SAMPLELIBRARY_EXPORT void decref_layer(layer*);
	SAMPLELIBRARY_EXPORT void incref_layer(layer*);
	SAMPLELIBRARY_EXPORT style_list* layer_get_styles(layer*);

	// Style list
	SAMPLELIBRARY_EXPORT int style_list_get_size(style_list*);
	SAMPLELIBRARY_EXPORT style* style_list_at(style_list*, int);
	SAMPLELIBRARY_EXPORT void style_list_remove(style_list*, int);
	SAMPLELIBRARY_EXPORT void style_list_insert(style_list*, int, style*);
	SAMPLELIBRARY_EXPORT void style_list_add(style_list*, style*);

	// Style
	SAMPLELIBRARY_EXPORT void decref_style(style*);
	SAMPLELIBRARY_EXPORT void incref_style(style*);

	// SymbolStyle
	SAMPLELIBRARY_EXPORT symbol_style* create_symbol_style();
	SAMPLELIBRARY_EXPORT const char* symbol_style_get_path(symbol_style*);
	SAMPLELIBRARY_EXPORT void symbol_style_set_path(symbol_style*, const char*);

	// LineStyle
	SAMPLELIBRARY_EXPORT line_style* create_line_style();
	SAMPLELIBRARY_EXPORT int line_style_get_width(line_style*);
	SAMPLELIBRARY_EXPORT void line_style_set_width(line_style*, int);
}


#endif C_INTERFACE_DEFINE
