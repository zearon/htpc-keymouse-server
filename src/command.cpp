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
#include <pthread.h>
#include <sys/types.h>

#include "log.h"
#include "command.h"
#include "network.h"
#include "gui.h"

using namespace std;

static xdo_t *xdo = NULL;
static pthread_t main_tid; 

static void init() {
  if (xdo == NULL) {
    // Such as :0. If null, uses the environment variable DISPLAY just like XOpenDisplay(NULL)
    xdo = xdo_new(NULL);
  }
}

static inline int logAndSendToClient(const std::function<void(std::ostream&)> &callback) {
  log(callback);
  sendMsgToClient(callback);
}

static inline int runSystemCmd(const char *instruction, const char *command) {
//   logln(instruction);
  logAndSendToClient( [instruction,command] (ostream &os) { os << instruction << ": " << command << endl; } ); 
  system(command);
}

void execInstruction(int instructionCode, char *instPayload, int payloadLen) {
//   log("Execute instruciton:", instructionCode);
//   logln(" with payload:", instPayload);
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
  case _COMMAND_KEY_EVENT:      // 0x09
    sendKeyEvent(instPayload);
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
  case _COMMAND_CHANGE_SOUND_DEVICE:    // 0x0a
    changeSoundDevice(instPayload);
    break;    
  case _COMMAND_HEART_BEAT:    // 0xff
    heartbeat(instPayload);
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
  logAndSendToClient( [text] (ostream &os) { os << "输入文本:" << text << endl; } ); 
  
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

static void changeSoundDevice(const char *payload) {
  int soundCard = * ((int*) payload);
  soundCard = ntohl(soundCard);
  const char *outputDevice = payload + 4;
  ostringstream cmds;
  cmds << "pacmd set-card-profile " << soundCard << " output:" << outputDevice;
  string cmd = cmds.str();
  runSystemCmd("切换声音输出设备", cmd.c_str());
}

static void sendKeypressEvent(const char *payload) {
  logAndSendToClient( [payload] (ostream &os) { os << "键盘按键:" << payload << endl; } ); 
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

static void sendKeyEvent(const char *payload) {
  int eventType = * ((int*) payload);  // 1 key_down, 2 key_up
  eventType = ntohl(eventType);
  const char *keyText = payload + 4;  
  const char* action = (eventType == 1) ? "按下键盘按键" : "松开键盘按键" ;
  logAndSendToClient( [action, keyText] (ostream &os) { os << action << keyText << endl; } ); 
  /**
   * Send key release (up) events for the given key sequence.
   *
   * @see xdo_send_keysequence_window
   */
//   int xdo_send_keysequence_window_up(const xdo_t *xdo, Window window,
//                          const char *keysequence, useconds_t delay);
//   int xdo_send_keysequence_window_down(const xdo_t *xdo, Window window,
//                            const char *keysequence, useconds_t delay);
  if (eventType == 1) {
    xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, keyText, 30);
  } else if (eventType == 2) {
    xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, keyText, 30);
  }
}

static void sendMouseMoveEvent(const char *payload) {  
  int delta_x = * ((int*) payload);
  int delta_y = * ((int*) (payload + 4));
  delta_x = ntohl(delta_x);
  delta_y = ntohl(delta_y);
  logAndSendToClient( [delta_x, delta_y](ostream &os) { os <<"移动鼠标光标 （" <<delta_x <<", " <<delta_y <<")" <<endl; } ); 
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
  
  logAndSendToClient( [button](ostream &os) { os << "点击鼠标：" << getMouseButtonName(button) << endl; } ); 
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
  
  logAndSendToClient( [button](ostream &os) { os << "双击鼠标：" << getMouseButtonName(button) << endl; } );
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
  logAndSendToClient( [action,button](ostream &os) { os << action << getMouseButtonName(button) << endl; } );
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

static int heartbeatOn = -1;
static int heartbeatMissed = 0;
static gint heartbeatSetInactiveStatusIcon(gpointer user_data) {
  ++heartbeatMissed;
  if (heartbeatMissed > 5) {    
    blink(-1); // yellow
    heartbeatOn = -1;
    return 0; // terminate this timer.
  } else {
    blink(0); // yellow
    return 1; // 1 for TRUE, which indicates the timer to call this function repeatedly. 
  }
}

static gint startHeartbeatSetInactiveStatusIcon(gpointer user_data) {
  blink(0); // yellow
  g_timeout_add(1000, GSourceFunc(heartbeatSetInactiveStatusIcon), NULL);
  return 0; // 0 for FALSE, which indicates a one time timer. 
}

static void heartbeat(const char *payload){
  if (heartbeatOn < 0) {
    // Init 
    heartbeatMissed = 0;
    g_timeout_add(500, GSourceFunc(startHeartbeatSetInactiveStatusIcon), NULL);
  }
  heartbeatOn = (++heartbeatOn) % 2;
  --heartbeatMissed;
  //logln("心跳 ", payload);
  blink(1);
}