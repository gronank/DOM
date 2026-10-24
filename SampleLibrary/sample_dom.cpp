#include "sample_dom.h"
#include "sample_c_interface.h"
#include "internal.hpp"
#include <array>
#include <map>
#include <typeindex>


type* type_of_style(style* style);

ce_module m
{
    .name = "SampleLibrary",
};

type style_t
{
    .name = "Style",
    .inc_ref = (inc_ref)&incref_style,
    .dec_ref = (dec_ref)&decref_style,
    .type_of = (type_of)&type_of_style,
    
};


// SymbolStyle
property pathProperty
{
    .name = "Path",
    .value_kind = string,
    .getter = {.STRING = (get_property_s)&symbol_style_get_path},
    .setter = {.STRING = (set_property_s)&symbol_style_set_path},
};

std::array symbolStyleProperties = { pathProperty };

type symbol_style_t
{
    .name = "SymbolStyle",
    .parent = &style_t,
    .constructor = (constructor)&create_symbol_style,
    .properties = {symbolStyleProperties.data(), (int)symbolStyleProperties.size()},
    .inc_ref = (inc_ref)&incref_style,
    .dec_ref = (dec_ref)&decref_style,
    .module = &m
};

// LineStyle
property widthProperty
{
    .name = "Width",
    .value_kind = integer,
    .getter = {.INTEGER = (get_property_i)&line_style_get_width},
    .setter = {.INTEGER = (set_property_i)&line_style_set_width},
};

std::array lineStyleProperties = { widthProperty };

type line_style_t
{
    .name = "LineStyle",
    .parent = &style_t,
    .constructor = (constructor)&create_line_style,
    .properties = {lineStyleProperties.data(), (int)lineStyleProperties.size()},
    .inc_ref = (inc_ref)&incref_style,
    .dec_ref = (dec_ref)&decref_style,
    .module = &m
};

list_info style_list_info
{
    .list_type = &style_t,
    .value_kind = api_type,
    .list_size = (list_size) &style_list_get_size,
    .list_at = {.TYPE = (list_at_t) &style_list_at},
    .list_remove = (list_remove) &style_list_remove,
    .list_insert = {.TYPE = (list_insert_t) &style_list_insert},
    .list_add = {.TYPE = (list_add_t) &style_list_add}
};

// StyleList
type style_list_t
{
    .name = "StyleList",
    .list_info = &style_list_info,
    .module = &m
};


// Layer
property stylesProperty
{
    .name = "Styles",
    .type = &style_list_t,
    .value_kind = api_type,
    .getter = {.TYPE = (get_property_t)&layer_get_styles}
};

std::array layerProperties = { stylesProperty };
type layer_t
{
    .name = "Layer",
    .constructor = (constructor)create_layer,
    .properties = {layerProperties.data(), (int)layerProperties.size()},
    .inc_ref = (inc_ref)&incref_layer,
    .dec_ref = (dec_ref)&decref_layer,
    .module = &m
};

// LayerList
list_info layer_list_info
{
     .list_type = &layer_t,
    .value_kind = api_type,
    .list_size = (list_size)&layer_list_get_size,
    .list_at = {.TYPE = (list_at_t)&layer_list_at},
    .list_remove = (list_remove)&layer_list_remove,
    .list_insert = {.TYPE = (list_insert_t)&layer_list_insert},
    .list_add = {.TYPE = (list_add_t)&layer_list_add}
};

type layer_list_t
{
    .name = "LayerList",
    .list_info = &layer_list_info,
    .module = &m
};

// View
property layersProperty
{
    .name = "Layers",
    .type = &layer_list_t,
    .value_kind = api_type,
    .getter = {.TYPE = (get_property_t)&view_get_layers}
};

std::array viewProperties = { layersProperty };

type view_t{
        .name = "View",
        .constructor = (constructor)&create_view,
        .properties = {viewProperties.data(), (int)viewProperties.size()},
        .inc_ref = (inc_ref)&incref_view,
        .dec_ref = (dec_ref)&decref_view,
        .module = &m
};

std::array types
{
    style_t,
    style_list_t,
    symbol_style_t,
    line_style_t,
    layer_t,
    layer_list_t,
    view_t
};

ce_module* load_module()
{
    m.types = { types.data(), (int)types.size() };
    return &m;
}

type* view_type()
{
    return &view_t;
}

std::map<std::type_index, type*> typeMap{ 
    {typeid(SymbolStyle),&symbol_style_t},
    {typeid(LineStyle), &line_style_t}};

type* type_of_style(style* s_p)
{
    RefView<Style> l = (ControlBlock*)s_p;
    auto* type = l.heldType();
    if (!type)
        return nullptr;
    auto it = typeMap.find(*type);
    if(it == typeMap.end())
        return nullptr;
    return it->second;
}
