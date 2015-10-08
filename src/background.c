#include <pebble.h>
#include "background.h"

BitmapLayer *Background_layer;
#define ANIME_NUM 2
GBitmap *hachune[ ANIME_NUM ];


void background_update( const struct tm* tick_time )
{
  bitmap_layer_set_bitmap( Background_layer, hachune[ time( NULL ) % ANIME_NUM ] );
}

void background_load( const Window *window_ptr, const GPoint point )
{
  hachune[ 0 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_HACHUNE01 );
  hachune[ 1 ] = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_HACHUNE02 );

  Background_layer = bitmap_layer_create( GRect( point.x, point.y, 144 - point.x, 169 - point.y ) );  // max 144 * 169
  layer_add_child( window_get_root_layer( window_ptr ), bitmap_layer_get_layer( Background_layer ) );
}

void background_unload()
{
  bitmap_layer_destroy( Background_layer );

  for ( int i = 0 ; i < ANIME_NUM ; i++ )
  {
    gbitmap_destroy( hachune[ i ] );
  }
}
