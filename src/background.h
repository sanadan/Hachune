#pragma once

extern void background_update( const struct tm* tick_time );
//extern void background_received_callback( DictionaryIterator *iterator, const void *context );
extern void background_load( const Window *window_ptr, const GPoint point );
extern void background_unload();
