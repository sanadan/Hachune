#include <pebble.h>
#include "pebble_date.h"
#include "pebble_time.h"
#include "battery.h"
#include "bluetooth.h"
#include "weather.h"
#include "background.h"
  
// Main window
Window *Main_window;


void tick_handler( struct tm *tick_time, TimeUnits units_changed )
{
  date_update( tick_time );
  time_update( tick_time );
  battery_update( tick_time );
  bluetooth_update( tick_time );
  weather_update( tick_time );
  background_update( tick_time );
}

void main_window_load( Window *window_ptr )
{
  background_load( window_ptr, GPoint( 0, 20 ) );  // max 144 * 144
  weather_load( window_ptr, GPoint( 4, 109 ) );  // x * 16
  bluetooth_load( window_ptr, GPoint( 131, 153 ) );  // 9 * 13
  battery_load( window_ptr, GPoint( 84, 1 ) ) ;  // x * 13
  time_load( window_ptr, GPoint( 4, 14 ) );  // x * 16
  date_load( window_ptr, GPoint( 4, 0 ) );  // x * 16
}

void main_window_unload( Window *window_ptr )
{
  date_unload();
  time_unload();
  battery_unload();
  bluetooth_unload();
  weather_unload();
  background_unload();
}

void inbox_received_callback( DictionaryIterator *iterator, void *context )
{
  APP_LOG( APP_LOG_LEVEL_INFO, "Inbox received" );

  weather_received_callback( iterator, context );
}

void inbox_dropped_callback( AppMessageResult reason, void *context )
{
  APP_LOG( APP_LOG_LEVEL_ERROR, "Inbox dropped!" );
}

void outbox_failed_callback( DictionaryIterator *iterator, AppMessageResult reason, void *context )
{
  APP_LOG( APP_LOG_LEVEL_ERROR, "Outbox send failed!" );
}

void outbox_sent_callback( DictionaryIterator *iterator, void *context )
{
  APP_LOG( APP_LOG_LEVEL_INFO, "Outbox send success" );
}

void init()
{
  // Register callbacks
  app_message_register_inbox_received( inbox_received_callback );
  app_message_register_inbox_dropped( inbox_dropped_callback );
  app_message_register_outbox_failed( outbox_failed_callback );
  app_message_register_outbox_sent( outbox_sent_callback );
  app_message_open( app_message_inbox_size_maximum(), app_message_outbox_size_maximum() );    

  // Create main Window element and assign to pointer
  Main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers( Main_window, ( WindowHandlers )
    {
      .load = main_window_load,
      .unload = main_window_unload
    }
  );
  
  // Show the Window on the watch, with animated=true
  window_stack_push( Main_window, true );
  
  // Register with TickTimerService
  tick_timer_service_subscribe( SECOND_UNIT, tick_handler );
}

static void deinit()
{
  // Destroy Window
  window_destroy( Main_window );
}

int main()
{
  init();
  app_event_loop();
  deinit();
}
