#pragma once

extern void date_update( const struct tm* tick_time );
// extern void date_received_callback( DictionaryIterator *iterator, const void *context );
extern void date_load( const Window *window_ptr, const GPoint point );
extern void date_unload();
