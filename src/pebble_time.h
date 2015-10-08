#pragma once
// "time.h" is already exists in standard C library.

extern void time_update( const struct tm* tick_time );
//extern void time_received_callback( DictionaryIterator *iterator, const void *context );
extern void time_load( const Window *window_ptr, const GPoint point );
extern void time_unload();
