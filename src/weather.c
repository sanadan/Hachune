#include <pebble.h>
#include "weather.h"

// Weather layer
TextLayer *weather_layer;


// Update wether info
void weather_update( struct tm *tick_time )
{
  if ( tick_time->tm_min % 30 != 0 ) return ;
  
  // Send message to weather.js
  DictionaryIterator *iter;
  app_message_outbox_begin( &iter );
  dict_write_uint8( iter, WEATHER_UPDATE, 0 );
  app_message_outbox_send();
}

// Callback from weather.js
void weather_received_callback( DictionaryIterator *iterator, void *context )
{
  // Store incoming information
  static char temperature_str[ 8 ];
  static char conditions_str[ 32 ];
  static char wind_speed_str[ 8 ];
  static char weather_str[ 32 ];
  
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL)
  {
    // Which key was received?
    switch(t->key)
    {
      case WEATHER_TEMPERATURE :  // temperature
        snprintf( temperature_str, sizeof ( temperature_str ), "%d C", ( int )t->value->int32 ) ;
        break;

      case WEATHER_CONDITION :  // condition
        snprintf( conditions_str, sizeof ( conditions_str ), "%s", t->value->cstring );
        break;
      
      case WEATHER_WIND_SPEED :  // wind speed
        snprintf( wind_speed_str, sizeof ( wind_speed_str ), "%s m", t->value->cstring );
        break;

      default:  // others
        APP_LOG( APP_LOG_LEVEL_ERROR, "Weather: Key %d not recognized!", ( int )t->key );
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  snprintf( weather_str, sizeof( weather_str ), "%s, %s, %s", temperature_str, conditions_str, wind_speed_str );
  text_layer_set_text( weather_layer, weather_str );
}

// Load weather settings
void weather_load( Window *window_ptr, GRect rect )
{
  weather_layer = text_layer_create( rect );
  text_layer_set_background_color( weather_layer, GColorClear );
  text_layer_set_text_color( weather_layer, GColorBlack );
  text_layer_set_text_alignment( weather_layer, GTextAlignmentRight );
  text_layer_set_text( weather_layer, "Loading..." );
  text_layer_set_font( weather_layer, fonts_get_system_font( FONT_KEY_GOTHIC_09 ) );
  layer_add_child( window_get_root_layer( window_ptr ), text_layer_get_layer( weather_layer ) );
}

void weather_unload()
{
  text_layer_destroy( weather_layer );
}
