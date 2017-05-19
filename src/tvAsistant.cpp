#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <map> 
#include <iostream>
#include "network.h"
#include "gui.h"
#include "log.h"

int main(int argc, char *argv[])
{  
  int oc;
  std::ostringstream cmdargs_oss;
  char *host = (char *) "0.0.0.0";
  char *port = (char *) "8888";
  char *wshost = (char *) "0.0.0.0";
  char *wsport = (char *) "8889";
  char *wsexecpath = (char *) "/var/www/tvremote/ws/tvremote-wsserver.js";
  if (argc < 5) {
    printf("Usage: tvAsistant -h [ip] -p [port] -w [websocket server ip] -q [websocket port] -e [websocket server path]\n  Default: ip=%s, port=%s,wssip=%s, wssport=%s, wsspath=%s\n", 
      host, port, wshost, wsport, wsexecpath);
    cmdargs_oss << "(默认) ";
  }
  
  char *textbuffer = new char[1024];
  logln("\n-----------------------------------------");
  logln("cmd path: ", argv[0]);
  getcwd(textbuffer, 1024);
  logln("  current working direcotry: ", textbuffer);
  delete[] textbuffer;
  
  while((oc = getopt(argc, argv, "h:p:w:q:e:")) != -1) {
    switch(oc) {
      case 'h':
        host = strdup(optarg);
        break;
      case 'p':
        port = strdup(optarg);
        break;
      case 'w':
        wshost = strdup(optarg);
        break;
      case 'q':
        wsport = strdup(optarg);
        break;
      case 'e':
        wsexecpath = strdup(optarg);
        break;
    }
  }
  
  cmdargs_oss << "-h " << host << " -p " << port << " -w " << wshost << " -q " << wsport << "\n -e " << wsexecpath;
  std::string defaultCmdArgs = cmdargs_oss.str();
  
  std::map<std::string, char*> opts;
  opts["host"] = host;
  opts["port"] = port;
  opts["wshost"] = wshost;
  opts["wsport"] = wsport;
  opts["wsexecpath"] = wsexecpath;
  opts["starup-args"] = (char*) defaultCmdArgs.c_str();
  
  log([&opts](std::ostream &os) { 
    os << "  arg count:" << std::dec << opts.size() << "\n"
       << "  host arg:" << opts["host"] << "\n"
       << "  port arg:" << opts["port"] << "\n"
       << "  ws host arg:" << opts["wshost"] << "\n"
       << "  ws port arg:" << opts["wsport"] << "\n"
       << "  ws exec path arg:" << opts["wsexecpath"] << std::endl;
  });
  
  XInitThreads();
  
  startServer(host, port);
  
  initGui(argc, argv, &opts);
  
  return 0;
}