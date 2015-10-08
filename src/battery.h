#pragma once

extern void battery_update( const struct tm* tick_time );
//extern void battery_received_callback( DictionaryIterator *iterator, const void *context );
extern void battery_load( const Window *window_ptr, const GPoint point );
extern void battery_unload();
