#ifndef _COMMAND_H
#define _COMMAND_H

#define _COMMAND_START_BROWSER 0x01
#define _COMMAND_QUIT_BROWSER 0x02
#define _COMMAND_INPUT_TEXT 0x03
#define _COMMAND_KEY_PRESS 0x04
#define _COMMAND_KEY_EVENT 0x09
#define _COMMAND_MOUSE_MOVE 0x05
#define _COMMAND_MOUSE_CLICK 0x06
#define _COMMAND_MOUSE_DOUBLE_CLICK 0x07
#define _COMMAND_MOUSE_EVENT 0x08

void execInstruction(int instructionCode, char *instPayload, int payloadLen);

static void startBrowser();
static void quitBrowser();
static void sendTextToFocus(const char *text);
static void sendKeypressEvent(const char *payload);
static void sendKeyEvent(const char *payload);
static void sendMouseMoveEvent(const char *payload);
static void sendMouseClickEvent(const char *payload);
static void sendMouseDoubleClickEvent(const char *payload);
static void sendMouseEvent(const char *payload);
#endif