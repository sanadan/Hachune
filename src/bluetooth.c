#include <pebble.h>
#include "bluetooth.h"

// Layer
BitmapLayer *Bluetooth_layer;
GBitmap *Bluetooth_icon;
GBitmap *Bluetooth_x_icon;
bool Bluetooth = false;


void bluetooth_update_connection( bool connected )
{
  Bluetooth = connected;

  if ( Bluetooth ) return;
  
  // Vibratoin if bluetoth is disconnected.
  static const uint32_t const segments[] = { 500, 500, 500, 500, 500 };
  VibePattern pattern = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH( segments ),
  };
  vibes_enqueue_custom_pattern( pattern );
}

void bluetooth_update( const struct tm* tick_time )
{
  if ( Bluetooth ) bitmap_layer_set_bitmap( Bluetooth_layer, Bluetooth_icon );
  else bitmap_layer_set_bitmap( Bluetooth_layer, Bluetooth_x_icon );
}

void bluetooth_load( const Window *window_ptr, const GPoint point )
{
  // Create bluetooth icon
  Bluetooth_layer = bitmap_layer_create( GRect( point.x, point.y, 9, 13 ) );
  layer_add_child( window_get_root_layer( window_ptr ), bitmap_layer_get_layer( Bluetooth_layer ) );
  Bluetooth_icon = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BLUETOOTH_ICON );
  Bluetooth_x_icon = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BLUETOOTH_X_ICON );
  
  // Subscribe bluetooth connection
  Bluetooth = bluetooth_connection_service_peek();
  bluetooth_connection_service_subscribe( bluetooth_update_connection );
}

void bluetooth_unload()
{
  bluetooth_connection_service_unsubscribe();
  gbitmap_destroy( Bluetooth_x_icon );
  gbitmap_destroy( Bluetooth_icon );
  bitmap_layer_destroy( Bluetooth_layer );
}
