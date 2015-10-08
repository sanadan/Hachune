#pragma once

extern void bluetooth_update( const struct tm* tick_time );
//extern void bluetooth_received_callback( DictionaryIterator *iterator, const void *context );
extern void bluetooth_load( const Window *window_ptr, const GPoint point );
extern void bluetooth_unload();
