#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include "network.h"
#include "gui.h"

int main(int argc, char *argv[])
{
  XInitThreads();
  startServer(8888);
  
  initGui(argc, argv);
  
  return 0;
}