#pragma once

#define WEATHER_UPDATE 10
#define WEATHER_CONDITION    11
#define WEATHER_TEMPERATURE  12
#define WEATHER_WIND_SPEED   13

extern void weather_update( struct tm* tick_time );
extern void weather_received_callback( DictionaryIterator *iterator, void *context );
extern void weather_load( Window *window_ptr, GRect rect );
extern void weather_unload();
