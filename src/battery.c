#include <pebble.h>
#include "battery.h"

// Layer
BitmapLayer *Battery_layer;
#define  BATTERY_NUM  11  
GBitmap *Battery_icon[ BATTERY_NUM ];
BatteryChargeState Battery_state;
BitmapLayer *Charge_layer;
GBitmap *Charge_icon[ 2 ];


static void battery_update_state( BatteryChargeState state )
{
  Battery_state = state;
}

void battery_update( const struct tm* tick_time )
{
  // Display battery icon
  bitmap_layer_set_bitmap( Battery_layer, Battery_icon[ Battery_state.charge_percent / 10 ] );

  // Display charge icon
  bitmap_layer_set_bitmap( Charge_layer, Charge_icon[ Battery_state.is_plugged ? 0 : 1 ] );
}

void battery_load( const Window *window_ptr, const GPoint point )
{
  // Create battery icon
  Battery_layer = bitmap_layer_create( GRect( point.x, point.y, 47, 13 ) );
  layer_add_child( window_get_root_layer( window_ptr ), bitmap_layer_get_layer( Battery_layer ) );
  Battery_icon[ 0 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_000 );
  Battery_icon[ 1 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_010 );
  Battery_icon[ 2 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_020 );
  Battery_icon[ 3 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_030 );
  Battery_icon[ 4 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_040 );
  Battery_icon[ 5 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_050 );
  Battery_icon[ 6 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_060 );
  Battery_icon[ 7 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_070 );
  Battery_icon[ 8 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_080 );
  Battery_icon[ 9 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_090 );
  Battery_icon[ 10 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATTERY_ICON_100 );

  // Create charge icon
  Charge_layer = bitmap_layer_create( GRect( point.x + 47 , point.y, 9, 13 ) );
  layer_add_child( window_get_root_layer( window_ptr ), bitmap_layer_get_layer( Charge_layer ) );
  Charge_icon[ 0 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_CHARGE_ICON_1 );
  Charge_icon[ 1 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_CHARGE_ICON_2 );
  
  // Subscribe battery state
  Battery_state = battery_state_service_peek();  // Get first state
  battery_state_service_subscribe( battery_update_state );
}

void battery_unload()
{
  // Unsubscribe battery state
  battery_state_service_unsubscribe();

  // Destroy charge icon
  for ( int i = 0 ; i < 2 ; i++ )
  {
    gbitmap_destroy( Charge_icon[ i ] );
  }
  bitmap_layer_destroy( Charge_layer );
  
  // Destroy battery icon
  for ( int i = 0 ; i < BATTERY_NUM ; i++ )
  {
    gbitmap_destroy( Battery_icon[ i ] );
  }
  bitmap_layer_destroy( Battery_layer );
}
