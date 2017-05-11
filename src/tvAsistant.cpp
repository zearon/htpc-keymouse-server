#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "gui.h"
#include "log.h"

int main(int argc, char *argv[])
{  
  int oc;
  char *host = (char *) "0.0.0.0";
  char *port = (char *) "8888";
  if (argc < 2) {
    printf("Usage: tvAsistant -h [ip] -p [port]\n  Default ip is %s and default port is %s\n", host, port);
  }
  
  char *textbuffer = new char[1024];
  logln("cmd path: ", argv[0]);
  getcwd(textbuffer, 1024);
  logln("current working direcotry: ", textbuffer);
  delete[] textbuffer;
  
  while((oc = getopt(argc, argv, "h:p:")) != -1) {
    switch(oc) {
      case 'h':
        host = strdup(optarg);
        break;
      case 'p':
        port = strdup(optarg);
        break;
    }
  }
  
  XInitThreads();
  
  startServer(host, port);
  
  initGui(argc, argv);
  
  return 0;
}