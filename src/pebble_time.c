#include <pebble.h>
#include "pebble_time.h"

// Time text layer
TextLayer *Time_layer;
GFont Time_font;


void time_update( const struct tm* tick_time )
{
  static char buffer[] = "00:00:00 xx";
  char minsec[] = "0000";

  if ( clock_is_24h_style() )
  {  // Use 24 hour format
    strftime( buffer, sizeof (buffer), "%H:%M:%S", tick_time);
  }
  else
  {  // Use 12 hour format
    strftime( buffer, sizeof ( buffer ), "%r", tick_time);
  }
   
  text_layer_set_text( Time_layer, buffer );

  strftime( minsec, sizeof ( minsec ), "%M%S", tick_time);
  if ( strcmp( minsec, "0000" ) == 0 )
  {  // An hour
    static const uint32_t const segments[] = { 250, 250, 250 };
    VibePattern pattern = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH( segments ),
    };
    vibes_enqueue_custom_pattern( pattern );
  }
}

void time_load( const Window *window_ptr, const GPoint point )
{
  Time_layer = text_layer_create( GRect( point.x, point.y, 144 - point.x, 34 ) );
  text_layer_set_background_color( Time_layer, GColorClear );
#ifdef PBL_COLOR
  text_layer_set_text_color( Time_layer, GColorBlack );
#else
  text_layer_set_text_color( Time_layer, GColorBlack );
#endif
  Time_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DIGITAL_32 ) );
//  text_layer_set_font( Time_layer, fonts_get_system_font( FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM ) );
  text_layer_set_font( Time_layer, Time_font );
  layer_add_child( window_get_root_layer( window_ptr ), text_layer_get_layer( Time_layer ) );
//  text_layer_set_text_alignment( Time_layer, GTextAlignmentLeft );
  text_layer_set_text_alignment( Time_layer, GTextAlignmentCenter );
}

void time_unload()
{
  fonts_unload_custom_font( Time_font );
  text_layer_destroy( Time_layer );
}
