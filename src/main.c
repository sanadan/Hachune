#include <pebble.h>
#include "weather.h"
  
// Main window
static Window *s_main_window;

// Time text layer
static TextLayer *s_time_layer;
static GFont s_time_font;

// Background layer
static BitmapLayer *s_background_layer;
//static GBitmap *s_background_bitmap;
#define ANIME_NUM 2
static GBitmap *hachune[ANIME_NUM];
static int count = 0;

// Bluetooth
static BitmapLayer *Bluetooth_layer;
static GBitmap *Bluetooth_icon;
static GBitmap *Bluetooth_x_icon;
bool Bluetooth = false;

// Battery
static BitmapLayer *Battery_layer;
#define  BATTERY_NUM  11  
static GBitmap *Battery_icon[ BATTERY_NUM ];
BatteryChargeState Battery_state;

// Charge icon layer
static BitmapLayer *Charge_layer;
static GBitmap *Charge_icon[2];

// Date layer
static TextLayer *Date_layer;
static GFont Date_font;


static void update_battery_state(BatteryChargeState state)
{
  Battery_state = state;
}

static void update_bluetooth(bool connected)
{
  Bluetooth = connected;
  if ( !Bluetooth )
  {
    static const uint32_t const segments[] = { 500, 500, 500, 500, 500 };
    VibePattern pattern = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH( segments ),
    };
    vibes_enqueue_custom_pattern( pattern );
  }
}

static void update_time() {
  // Get a tm structure
  time_t seconds = time(NULL); 
  struct tm *tick_time = localtime(&seconds);

  // Create a long-lived buffer
  static char buffer[] = "00:00:00";
  static char date_str[] = "0000/00/00 xxx";
  static char minsec[] = "0000";

  // Write the current date
  strftime(date_str, sizeof (date_str), "%F %a", tick_time);

  // Write the current time
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof (buffer), "%H:%M:%S", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof (buffer), "%I:%M:%S", tick_time);
  }
  
  // Display Background
  count++;
  if ( count >= 100 ) count = 0;
  bitmap_layer_set_bitmap(s_background_layer, hachune[seconds % ANIME_NUM]);

  // Display bluetooth icon
  if ( Bluetooth ) bitmap_layer_set_bitmap(Bluetooth_layer, Bluetooth_icon);
  else bitmap_layer_set_bitmap(Bluetooth_layer, Bluetooth_x_icon);

  // Display battery icon
  bitmap_layer_set_bitmap( Battery_layer, Battery_icon[ Battery_state.charge_percent / 10 ] );

  // Display charge icon
  bitmap_layer_set_bitmap( Charge_layer, Charge_icon[ Battery_state.is_plugged ? 0 : 1 ] );

  // Display date
  text_layer_set_text(Date_layer, date_str);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);

  // Jihou
  strftime( minsec, sizeof ( minsec ), "%M%S", tick_time);
  if ( strcmp( minsec, "0000" ) == 0 )
  {  // An hour
    static const uint32_t const segments[] = { 250, 250, 250 };
    VibePattern pattern = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH( segments ),
    };
    vibes_enqueue_custom_pattern( pattern );
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  weather_update( tick_time );
}

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  //s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  hachune[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HACHUNE01);
  hachune[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HACHUNE02);

  // Create background
  s_background_layer = bitmap_layer_create(GRect(0, 20, 144, 149));  // max 144x169
  bitmap_layer_set_bitmap(s_background_layer, hachune[0]);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create bluetooth icon
  Bluetooth_layer = bitmap_layer_create( GRect( 83, 0, 9, 13 ) );
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(Bluetooth_layer));
  Bluetooth_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_ICON);
  Bluetooth_x_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_X_ICON);
  
  // Create battery icon
  Battery_layer = bitmap_layer_create( GRect( 92, 0, 43, 13 ) );
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(Battery_layer));
  Battery_icon[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_000);
  Battery_icon[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_010);
  Battery_icon[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_020);
  Battery_icon[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_030);
  Battery_icon[4] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_040);
  Battery_icon[5] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_050);
  Battery_icon[6] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_060);
  Battery_icon[7] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_070);
  Battery_icon[8] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_080);
  Battery_icon[9] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_090);
  Battery_icon[10] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_ICON_100);

  // Create charge icon
  Charge_layer = bitmap_layer_create(GRect( 135, 0, 9, 13));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(Charge_layer));
  Charge_icon[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_ICON_1);
  Charge_icon[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_ICON_2);
  
  // Create time TextLayer
  //s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  s_time_layer = text_layer_create(GRect(4, 28, 140, 20));
#ifdef PBL_COLOR
  //text_layer_set_background_color(s_time_layer, GColorDarkGray);
  text_layer_set_background_color(s_time_layer, GColorClear);
  //text_layer_set_text_color(s_time_layer, GColorBrightGreen);
  text_layer_set_text_color(s_time_layer, GColorDarkGreen);
#else
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
#endif
//  text_layer_set_text(s_time_layer, "00:00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  // Create GFont
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_28));
  // Apply to TextLayer
  //text_layer_set_font(s_time_layer, s_time_font);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create date TextLayer
  Date_layer = text_layer_create(GRect(4, 10, 140, 20));
  text_layer_set_background_color(Date_layer, GColorClear);
#ifdef PBL_COLOR
  text_layer_set_text_color(Date_layer, GColorDarkGreen);
#else
  text_layer_set_text_color(Date_layer, GColorBlack);
#endif
//  text_layer_set_text(Date_layer, "0000-00-00");
  text_layer_set_font(Date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(Date_layer));
  text_layer_set_text_alignment(Date_layer, GTextAlignmentCenter);

  // Subscribe bluetooth connection
  Bluetooth = bluetooth_connection_service_peek();
  bluetooth_connection_service_subscribe(update_bluetooth) ;

  // Subscribe battery state
  Battery_state = battery_state_service_peek();
  battery_state_service_subscribe( update_battery_state );

  // Weather
  weather_load( window, GRect( 0, 158, 136, 11 ) );
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Weather
  weather_unload();
  
  // Unsubscribe battery state
  battery_state_service_unsubscribe();
  
  // Unsubscribe bluetooth connection
  bluetooth_connection_service_unsubscribe();
  
  // Destroy charge icon
  for ( int i = 0 ; i < 2 ; i++ )
  {
    gbitmap_destroy(Charge_icon[i]);
  }
  
  // Destroy battery icon
  for ( int i = 0 ; i < BATTERY_NUM ; i++ )
  {
    gbitmap_destroy(Battery_icon[i]);
  }
  bitmap_layer_destroy(Battery_layer);
  
  // Destroy bluetooth icon
  gbitmap_destroy(Bluetooth_x_icon);
  gbitmap_destroy(Bluetooth_icon);
  bitmap_layer_destroy(Bluetooth_layer);
  
  // Destroy GBitmap
  //gbitmap_destroy(s_background_bitmap);
  for ( int i = 0 ; i < ANIME_NUM ; i++ )
  {
    gbitmap_destroy(hachune[i]);
  }
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

  // Unload date font
  fonts_unload_custom_font(Date_font);
  
  // Destroy TextLayer
  text_layer_destroy(Date_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void inbox_received_callback( DictionaryIterator *iterator, void *context )
{
  APP_LOG( APP_LOG_LEVEL_INFO, "Inbox received" );

  weather_received_callback( iterator, context );
}

static void inbox_dropped_callback( AppMessageResult reason, void *context )
{
  APP_LOG( APP_LOG_LEVEL_ERROR, "Inbox dropped!" );
}

static void outbox_failed_callback( DictionaryIterator *iterator, AppMessageResult reason, void *context )
{
  APP_LOG( APP_LOG_LEVEL_ERROR, "Outbox send failed!" );
}

static void outbox_sent_callback( DictionaryIterator *iterator, void *context )
{
  APP_LOG( APP_LOG_LEVEL_INFO, "Outbox send success" );
}

static void init() {
  // Register callbacks
  app_message_register_inbox_received( inbox_received_callback );
  app_message_register_inbox_dropped( inbox_dropped_callback );
  app_message_register_outbox_failed( outbox_failed_callback );
  app_message_register_outbox_sent( outbox_sent_callback );
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());    

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
