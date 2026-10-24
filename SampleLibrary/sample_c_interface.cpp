#include "internal.hpp"
#include "sample_c_interface.h"

view* create_view()
{
	Ref<View> v = new View();
	return (view*)std::move(v).grab();
}

void decref_view(view* v_p)
{
	Ref<View> v = (ControlBlock*)v_p;
	// Destruction of v
}

void incref_view(view* v_p)
{
	RefView<View> v = (ControlBlock*)v_p;
	v.incRef();
}

layer_list* view_get_layers(view* v_p)
{
	RefView<View> v = (ControlBlock*)v_p;
	return (layer_list*)&v->layers;
}

int layer_list_get_size(layer_list* ll_p)
{
	const auto& layerList = *(decltype(View::layers)*)ll_p;
	return (int)layerList.size();
}

layer* layer_list_at(layer_list* ll_p, int i)
{
	const auto& layerList = *(decltype(View::layers)*)ll_p;
	Ref<Layer> layerRef = layerList[i];
	return (layer*)std::move(layerRef).grab();
}

void layer_list_remove(layer_list* ll_p, int i)
{
	auto& layerList = *(decltype(View::layers)*)ll_p;
	layerList.erase(std::next(layerList.begin(), i));
}

void layer_list_insert(layer_list* ll_p, int i, layer* l_p)
{
	auto& layerList = *(decltype(View::layers)*)ll_p;
	RefView<Layer> layerRef = (ControlBlock*)l_p;
	layerList.insert(std::next(layerList.begin(), i), std::move(layerRef));

}

void layer_list_add(layer_list* ll_p, layer* l_p)
{
	auto& layerList = *(decltype(View::layers)*)ll_p;
	RefView<Layer> layerRef = (ControlBlock*)l_p;
	layerList.emplace_back(std::move(layerRef));
}

layer* create_layer()
{
	Ref<Layer> l = new Layer();
	return (layer*)std::move(l).grab();
}

void decref_layer(layer* l_p)
{
	Ref<Layer> l = (ControlBlock*)l_p;
	// Destruction of l
}

void incref_layer(layer* l_p)
{
	RefView<layer> l = (ControlBlock*)l_p;
	l.incRef();
}

style_list* layer_get_styles(layer* l_p)
{
	RefView<Layer> l = (ControlBlock*)l_p;
	return (style_list*)&l->styles;
}

int style_list_get_size(style_list* sl_p)
{
	const auto& styleList = *(decltype(Layer::styles)*)sl_p;
	return (int)styleList.size();
}

style* style_list_at(style_list* sl_p, int i)
{
	const auto& styleList = *(decltype(Layer::styles)*)sl_p;
	Ref<Style> styleRef = styleList[i];
	return (style*)std::move(styleRef).grab();
}

void style_list_remove(style_list* sl_p, int i)
{
	auto& layerList = *(decltype(Layer::styles)*)sl_p;
	layerList.erase(std::next(layerList.begin(), i));
}

void style_list_insert(style_list* sl_p, int i, style* s_p)
{
	auto& styleList = *(decltype(Layer::styles)*)sl_p;
	RefView<Style> styleRef = (ControlBlock*)s_p;
	styleList.insert(std::next(styleList.begin(), i), std::move(styleRef));
}

void style_list_add(style_list* ll_p, style* l_p)
{
	auto& styleList = *(decltype(Layer::styles)*)ll_p;
	RefView<Style> styleRef = (ControlBlock*)l_p;
	styleList.emplace_back(std::move(styleRef));
}

void decref_style(style* s_p)
{
	Ref<Style> s = (ControlBlock*)s_p;
}

void incref_style(style* s_p)
{
	RefView<Style> s = (ControlBlock*)s_p;
	s.incRef();
}

symbol_style* create_symbol_style()
{
	Ref<SymbolStyle> ss = new SymbolStyle();
	return (symbol_style*)std::move(ss).grab();
}

const char* symbol_style_get_path(symbol_style* ss_p)
{
	RefView<SymbolStyle> ss = (ControlBlock*)ss_p;
	return ss->path.c_str();
}

void symbol_style_set_path(symbol_style* ss_p, const char* path)
{
	RefView<SymbolStyle> ss = (ControlBlock*)ss_p;
	ss->path = path;
}

line_style* create_line_style()
{
	Ref<LineStyle> ls = new LineStyle();
	return (line_style*)std::move(ls).grab();
}

int line_style_get_width(line_style* ls_p)
{
	RefView<LineStyle> ls = (ControlBlock*)ls_p;
	return (int)ls->width;
}

void line_style_set_width(line_style* ls_p, int width)
{
	RefView<LineStyle> ls = (ControlBlock*)ls_p;
	ls->width = width;
}
