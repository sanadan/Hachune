#pragma once

#define WEATHER_CONDITION    10
#define WEATHER_TEMPERATURE  11

extern void weather_received_callback( DictionaryIterator *iterator, void *context );
extern void weather_load( Window *window_ptr );
extern void weather_unload();
