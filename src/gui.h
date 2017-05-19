#ifndef _GUI_H
#define _GUI_H

#include <map> 

int initGui(int argc, char *argv[], std::map<std::string, char*> *opts_);
int appendMessage(const char *msg);
int blink(int on);

#endif