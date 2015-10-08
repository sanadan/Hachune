#pragma once

#define WEATHER_UPDATE 10
#define WEATHER_CONDITION    11
#define WEATHER_TEMPERATURE  12
#define WEATHER_WIND_SPEED   13
#define WEATHER_CITY         14

extern void weather_update( const struct tm* tick_time );
extern void weather_received_callback( DictionaryIterator *iterator, const void *context );
extern void weather_load( const Window *window_ptr, const GPoint point );
extern void weather_unload();
