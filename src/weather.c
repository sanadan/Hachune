#include <pebble.h>
#include "weather.h"

// Weather layer
TextLayer *Conditions_layer;
TextLayer *Temperature_layer;
TextLayer *Wind_speed_layer;
TextLayer *City_layer;
GFont Weather_font;

// Update wether info
void weather_update( const struct tm *tick_time )
{
  if ( tick_time->tm_min % 30 != 0 ) return ;
  
  // Send message to weather.js
  DictionaryIterator *iter;
  app_message_outbox_begin( &iter );
  dict_write_uint8( iter, WEATHER_UPDATE, 0 );
  app_message_outbox_send();
}

// Callback from weather.js
void weather_received_callback( DictionaryIterator *iterator, const void *context )
{
  // Store incoming information
  static char temperature_str[ 8 ];
  static char conditions_str[ 32 ];
  static char wind_speed_str[ 8 ];
  static char city_str[ 32 ];
  
  Tuple *t = dict_read_first(iterator);

  // For all items
  while( t != NULL )
  {
    // Which key was received?
    switch ( t->key )
    {
      case WEATHER_TEMPERATURE :  // temperature
        snprintf( temperature_str, sizeof ( temperature_str ), "%d C", ( int )t->value->int32 ) ;
        text_layer_set_text( Temperature_layer, temperature_str ) ;
        break;

      case WEATHER_CONDITION :  // condition
        snprintf( conditions_str, sizeof ( conditions_str ), "%s", t->value->cstring );
        text_layer_set_text( Conditions_layer, conditions_str ) ;
        break;
      
      case WEATHER_WIND_SPEED :  // wind speed
        snprintf( wind_speed_str, sizeof ( wind_speed_str ), "%s m", t->value->cstring );
        text_layer_set_text( Wind_speed_layer, wind_speed_str ) ;
        break;

      case WEATHER_CITY :  // city
        snprintf( city_str, sizeof ( city_str ), "%s", t->value->cstring );
        text_layer_set_text( City_layer, city_str );
        break;
      
      default:  // others
        APP_LOG( APP_LOG_LEVEL_ERROR, "Weather: Key %d not recognized!", ( int )t->key );
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
}

void weather_load_sub( TextLayer **layer, const Window *window_ptr, const GRect rect )
{
  *layer = text_layer_create( rect );
  text_layer_set_background_color( *layer, GColorClear );
  text_layer_set_text_color( *layer, GColorBlack );
  text_layer_set_text_alignment( *layer, GTextAlignmentLeft );
//  text_layer_set_font( *layer, fonts_get_system_font( FONT_KEY_GOTHIC_14_BOLD ) );
  text_layer_set_font( *layer, Weather_font );
  layer_add_child( window_get_root_layer( window_ptr ), text_layer_get_layer( *layer ) );
}

void weather_load( const Window *window_ptr, const GPoint point )
{
  int size_x = 136 - point.x;
  Weather_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_DIGITAL_12 ) );
  weather_load_sub( &Wind_speed_layer, window_ptr, GRect( point.x, point.y + 42, size_x, 12 ) );
  weather_load_sub( &Temperature_layer, window_ptr, GRect( point.x, point.y + 28, size_x, 12 ) );
  weather_load_sub( &Conditions_layer, window_ptr, GRect( point.x, point.y + 14, size_x, 12 ) );
  weather_load_sub( &City_layer, window_ptr, GRect( point.x, point.y, size_x, 14 ) );
}

void weather_unload()
{
  text_layer_destroy( City_layer );
  text_layer_destroy( Conditions_layer );
  text_layer_destroy( Temperature_layer );
  text_layer_destroy( Wind_speed_layer );
  fonts_unload_custom_font( Weather_font );
}
