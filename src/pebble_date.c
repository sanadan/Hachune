#include <pebble.h>
#include "pebble_date.h"

// Date layer
TextLayer *Date_layer;
static GFont Date_font;


void date_update( const struct tm* tick_time )
{
  static char date_str[] = "0000-00-00 xxx";
  strftime( date_str, sizeof ( date_str ), "%F %a", tick_time );
  text_layer_set_text( Date_layer, date_str );
}

void date_load( const Window *window_ptr, const GPoint point )
{
  Date_layer = text_layer_create( GRect( point.x, point.y, 136 - point.x, 14 ) ) ;
  text_layer_set_background_color( Date_layer, GColorClear );
#ifdef PBL_COLOR
  text_layer_set_text_color( Date_layer, GColorBlack );
#else
  text_layer_set_text_color( Date_layer, GColorBlack );
#endif
  Date_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DIGITAL_12 ) );
//  text_layer_set_font( Date_layer, fonts_get_system_font( FONT_KEY_GOTHIC_14_BOLD ) );
  text_layer_set_font( Date_layer, Date_font );
  layer_add_child( window_get_root_layer( window_ptr ), text_layer_get_layer( Date_layer ) );
  text_layer_set_text_alignment( Date_layer, GTextAlignmentLeft );
}

void date_unload()
{
  fonts_unload_custom_font( Date_font );
  text_layer_destroy( Date_layer );
}
