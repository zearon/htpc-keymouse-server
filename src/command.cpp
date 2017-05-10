#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkclipboard.h>
#include <arpa/inet.h>
// libxdo.so is implemented in C.
//#include <xdo.h>
extern "C" {
#include <xdo.h>
}
#include <stdlib.h>
#include <unistd.h>
#include "log.h"

#include <pthread.h>
#include <sys/types.h>

#include "command.h"
#include "network.h"

static xdo_t *xdo = NULL;
static pthread_t main_tid; 

static void init() {
  if (xdo == NULL) {
    // Such as :0. If null, uses the environment variable DISPLAY just like XOpenDisplay(NULL)
    xdo = xdo_new(NULL);
  }
}

static inline int runSystemCmd(const char *instruction, const char *command) {
  logln(instruction);
  system(command);
}

void execInstruction(int instructionCode, char *instPayload, int payloadLen) {
//   log("Execute instruciton:", instructionCode);
//   log(" with payload:", instPayload);
  init();
  
  switch (instructionCode) {
  case _COMMAND_START_BROWSER:  // 0x01
    startBrowser();
    break;
  case _COMMAND_QUIT_BROWSER:   // 0x02
    quitBrowser();
    break;
  case _COMMAND_INPUT_TEXT:     // 0x03
    sendTextToFocus(instPayload);
    break;
  case _COMMAND_KEY_PRESS:      // 0x04
    sendKeypressEvent(instPayload);
    break;
  case _COMMAND_MOUSE_MOVE:      // 0x05
    sendMouseMoveEvent(instPayload);
    break;
  case _COMMAND_MOUSE_CLICK:    // 0x06
    sendMouseClickEvent(instPayload);
    break;
  case _COMMAND_MOUSE_DOUBLE_CLICK: // 0x07
    sendMouseDoubleClickEvent(instPayload);
    break;
  case _COMMAND_MOUSE_EVENT:    // 0x08
    sendMouseEvent(instPayload);
    break;    
  default:
    break;
  }
}
 
static void *keypress(void *arg) {
//   while (1) {
//     //xdo_send_keysequence_window(xdo, CURRENTWINDOW, "c", 0);
//     //xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+v", 30);
//     sleep(1);
//   }
}

static void sendTextToFocus(const char *text) {
  logln("输入文本:", text);
  
  GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);  
  //logln("clipboard:", (long)clipboard);
  gtk_clipboard_set_text(clipboard, text, -1);
  
  xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+v", 30);
  //pthread_create(&main_tid, NULL, keypress, NULL);
}

static void startBrowser() {
  runSystemCmd("启动浏览器", "google-chrome &");
}

static void quitBrowser() {
  runSystemCmd("关闭浏览器", "killall chrome");
}

static void sendKeypressEvent(const char *payload) {
  logln("键盘按键:", payload);
  /**
   * Send a keysequence to the specified window.
   *
   * This allows you to send keysequences by symbol name. Any combination
   * of X11 KeySym names separated by '+' are valid. Single KeySym names
   * are valid, too.
   *
   * Examples:
   *   "l"
   *   "semicolon"
   *   "alt+Return"
   *   "Alt_L+Tab"
   *
   * If you want to type a string, such as "Hello world." you want to instead
   * use xdo_enter_text_window.
   *
   * @param window The window you want to send the keysequence to or
   *   CURRENTWINDOW
   * @param keysequence The string keysequence to send.
   * @param delay The delay between keystrokes in microseconds.
   */
  // int xdo_send_keysequence_window(const xdo_t *xdo, Window window,
  //                     const char *keysequence, useconds_t delay);
  xdo_send_keysequence_window(xdo, CURRENTWINDOW, payload, 0);
}

static void sendMouseMoveEvent(const char *payload) {  
  int delta_x = * ((int*) payload);
  int delta_y = * ((int*) (payload + 4));
  delta_x = ntohl(delta_x);
  delta_y = ntohl(delta_y);
  log("移动鼠标光标 （", delta_x);
  log(",", delta_y);
  logln(")");
  /**
   * Move the mouse relative to it's current position.
   *
   * @param x the distance in pixels to move on the X axis.
   * @param y the distance in pixels to move on the Y axis.
   */
  // int xdo_move_mouse_relative(const xdo_t *xdo, int x, int y);
  xdo_move_mouse_relative(xdo, delta_x, delta_y);
}

static inline const char* getMouseButtonName(int buttonCode) {
  const char *buttonName = "";
  switch (buttonCode) {
  case 2:
    buttonName = "中键";
    break;
  case 3:
    buttonName = "右键";
    break;
  case 4:
    buttonName = "滚轮向上";
    break;
  case 5:
    buttonName = "滚轮向下";
    break;
  default:
    buttonName = "左键";
    break;
  }
  return buttonName;
}

static void sendMouseClickEvent(const char *payload) {
  int button = * ((int*) payload);
  button = ntohl(button);
  if (button < 1 || button > 5) {
    logln("Invalid mouse button code. It should be within [1,5]");
    return;
  }
  
  logln("点击鼠标：", getMouseButtonName(button));  
  /**
   * Send a mouse press (aka mouse down) for a given button at the current mouse
   * location.
   *
   * @param window The window you want to send the event to or CURRENTWINDOW
   * @param button The mouse button. Generally, 1 is left, 2 is middle, 3 is
   *    right, 4 is wheel up, 5 is wheel down.
   */
  //int xdo_mouse_down(const xdo_t *xdo, Window window, int button);
  
  /**
   * Send a click for a specific mouse button at the current mouse location.
   *
   * @param window The window you want to send the event to or CURRENTWINDOW
   * @param button The mouse button. Generally, 1 is left, 2 is middle, 3 is
   *    right, 4 is wheel up, 5 is wheel down.
   */
  //int xdo_click_window(const xdo_t *xdo, Window window, int button);
  xdo_click_window(xdo, CURRENTWINDOW, button);
}

static void sendMouseDoubleClickEvent(const char *payload) {
  int button = * ((int*) payload);
  button = ntohl(button);
  if (button < 1 || button > 5) {
    logln("Invalid mouse button code. It should be within [1,5]");
    return;
  }
  
  logln("双击鼠标：", getMouseButtonName(button));  
  /**
   * Send a one or more clicks for a specific mouse button at the current mouse
   * location.
   *
   * @param window The window you want to send the event to or CURRENTWINDOW
   * @param button The mouse button. Generally, 1 is left, 2 is middle, 3 is
   *    right, 4 is wheel up, 5 is wheel down.
   */
//   int xdo_click_window_multiple(const xdo_t *xdo, Window window, int button,
//                          int repeat, useconds_t delay);
  xdo_click_window_multiple(xdo, CURRENTWINDOW, button, 2, 30);
}

static void sendMouseEvent(const char *payload) {
  int button = * ((int*) payload);
  int eventType = * ((int*) (payload + 4));  // 1 mouse_down, 2 mouse_up
  button = ntohl(button);
  eventType = ntohl(eventType);
  if (button < 1 || button > 5) {
    logln("Invalid mouse button code. It should be within [1,5]");
    return;
  }
  if (eventType < 1 || eventType > 5) {
    logln("Invalid mouse event code. It should 1 for mouse down or 2 for mouse up");
    return;
  }
  const char* action = (eventType == 1) ? "按下鼠标" : "松开鼠标" ;  
  logln(action, getMouseButtonName(button));
  /**
   * Send a mouse press (aka mouse down) for a given button at the current mouse
   * location.
   *
   * @param window The window you want to send the event to or CURRENTWINDOW
   * @param button The mouse button. Generally, 1 is left, 2 is middle, 3 is
   *    right, 4 is wheel up, 5 is wheel down.
   */
//   int xdo_mouse_down(const xdo_t *xdo, Window window, int button);
//   int xdo_mouse_up(const xdo_t *xdo, Window window, int button);
  if (eventType == 1) {
    xdo_mouse_down(xdo, CURRENTWINDOW, button);
  } else if (eventType == 2) {
    xdo_mouse_up(xdo, CURRENTWINDOW, button);
  }
}