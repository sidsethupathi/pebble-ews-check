#include <pebble.h>

#define NUM_ROWS 4
#define SPACING 29

Window *window;
TextLayer * name_layer[NUM_ROWS];
TextLayer * data_layer[NUM_ROWS];

static char name0[10];
static char data0[10];
static char name1[10];
static char data1[10];
static char name2[10];
static char data2[10];
static char name3[10];
static char data3[10];

static bool dataInited;

enum {
  MSG_KEY_INIT = 0x0,
  MSG_KEY_NAME0 = 0x01,
  MSG_KEY_DATA0 = 0x02,
  MSG_KEY_NAME1 = 0x03,
  MSG_KEY_DATA1 = 0x04,
  MSG_KEY_NAME2 = 0x05,
  MSG_KEY_DATA2 = 0x06,
  MSG_KEY_NAME3 = 0x07,
  MSG_KEY_DATA3 = 0x08,
};

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
 	APP_LOG(APP_LOG_LEVEL_DEBUG, "Received msg");
  Tuple *init_tuple = dict_find(iter, MSG_KEY_INIT);
  
  Tuple *name0_tuple = dict_find(iter, MSG_KEY_NAME0);
  Tuple *data0_tuple = dict_find(iter, MSG_KEY_DATA0);
  Tuple *name1_tuple = dict_find(iter, MSG_KEY_NAME1);
  Tuple *data1_tuple = dict_find(iter, MSG_KEY_DATA1);
  Tuple *name2_tuple = dict_find(iter, MSG_KEY_NAME2);
  Tuple *data2_tuple = dict_find(iter, MSG_KEY_DATA2);
  Tuple *name3_tuple = dict_find(iter, MSG_KEY_NAME3);
  Tuple *data3_tuple = dict_find(iter, MSG_KEY_DATA3);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting layer data now");
  
  if (name0_tuple) {
      strncpy(name0, name0_tuple->value->cstring, 10);
      text_layer_set_text(name_layer[0], name0);
  }
  
  if (data0_tuple) {
      strncpy(data0, data0_tuple->value->cstring, 10);
      text_layer_set_text(data_layer[0], data0);
  }
  
  if (name1_tuple) {
      strncpy(name1, name1_tuple->value->cstring, 10);
      text_layer_set_text(name_layer[1], name1);
  }
  
  if (data1_tuple) {
      strncpy(data1, data1_tuple->value->cstring, 10);
      text_layer_set_text(data_layer[1], data1);
  }
  
  if (name2_tuple) {
      strncpy(name2, name2_tuple->value->cstring, 10);
      text_layer_set_text(name_layer[2], name2);
  }
  
  if (data2_tuple) {
      strncpy(data2, data2_tuple->value->cstring, 10);
      text_layer_set_text(data_layer[2], data2);
  }
  
  if (name3_tuple) {
      strncpy(name3, name3_tuple->value->cstring, 10);
      text_layer_set_text(name_layer[3], name3);
  }
  
  if (data3_tuple) {
      strncpy(data3, data3_tuple->value->cstring, 10);
      text_layer_set_text(data_layer[3], data3);
  }
   
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped! Reason: %d - %s", reason, translate_error(reason));
}

void handle_init(void) {
  // set up AppMessage for comm with phone
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_open(256, 256);

  // Create a window and text layer

	window = window_create();
  int i;
  
  for(i = 0; i < NUM_ROWS; i++) {
    name_layer[i] = text_layer_create(GRect(0, SPACING*i, 134, SPACING));
    data_layer[i] = text_layer_create(GRect(90, SPACING*i, 54, SPACING));
  	
  	// Set the text, font, and text alignment
  	text_layer_set_text(name_layer[i], "Loading...");
  	text_layer_set_font(name_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_24));
  	text_layer_set_text_alignment(name_layer[i], GTextAlignmentLeft);
    
    text_layer_set_text(data_layer[i], "xx open");
  	text_layer_set_font(data_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_24));
  	text_layer_set_text_alignment(data_layer[i], GTextAlignmentRight);
  	
  	// Add the text layer to the window
  	layer_add_child(window_get_root_layer(window), text_layer_get_layer(name_layer[i]));
  	layer_add_child(window_get_root_layer(window), text_layer_get_layer(data_layer[i]));
  }
	

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed initial window and layes!");
}

void handle_deinit(void) {
	// Destroy the text layer
  int i;
  for(i = 0; i < NUM_ROWS; i++) {
    text_layer_destroy(name_layer[i]);
   	text_layer_destroy(data_layer[i]); 
  }
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
