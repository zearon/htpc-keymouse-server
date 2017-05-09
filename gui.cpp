#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"

#define MESSAGE_LIST_LENGTH 5

static GtkTextView *msgTextView = NULL;
static GtkTextBuffer *msgTextBuffer = NULL;
static GtkScrolledWindow *scrollwindow = NULL;

static int msgBufferMsgCount = 0;
/*
static void tray_on_click(GtkStatusIcon *tray,gpointer data) {
  gtk_widget_show_all(GTK_WIDGET(data));
  gtk_window_present(GTK_WINDOW(data));
  gtk_status_icon_set_visible(tray,FALSE);
}

static void tray_on_menu(GtkStatusIcon *tray,gpointer data) {
  printf("Popup Menu\n");
}

static void hide_window(GtkWidget *widget,GdkEventWindowState *event,gpointer data) {
  if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && 
    event->new_window_state == GDK_WINDOW_STATE_ICONIFIED) {
    gtk_widget_hide(widget); //gtk_widget_hide_all(widget);
    gtk_status_icon_set_visible(GTK_STATUS_ICON(data),TRUE);
  }
}
*/

static void onShowLogButton(GtkWidget *widget, gpointer data) {
  printf("Show log\n");
  //system("tail -f ./log.txt");
  system("gnome-terminal -x tail -f ./log.txt");
}

int initGui(int argc, char *argv[]) {  
  gtk_init(&argc, &argv);  
  
  GtkWidget *window;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "TV Assistant");
  gtk_widget_set_size_request(window, 200, 100);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
  
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  
  msgTextView = (GtkTextView*) gtk_text_view_new();  // 创建文本框
  msgTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msgTextView));  // 获得文本框缓冲区
  gtk_text_view_set_editable(msgTextView, FALSE);
  gtk_text_view_set_cursor_visible(msgTextView, FALSE);
  
  scrollwindow = (GtkScrolledWindow*) gtk_scrolled_window_new(NULL, NULL); // 创建滚动窗口
  gtk_container_add(GTK_CONTAINER(scrollwindow), (GtkWidget*) msgTextView);
  
  GtkWidget *showLogBtn = gtk_button_new_with_label ("显示日志 | Show Log");  //显示日志按钮
  g_signal_connect(G_OBJECT(showLogBtn), "clicked", G_CALLBACK(onShowLogButton),NULL);
  
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  // 垂直布局容器  
  gtk_container_add(GTK_CONTAINER(vbox), (GtkWidget*) scrollwindow);
  gtk_container_add(GTK_CONTAINER(vbox), showLogBtn);
  
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  /*
  //GtkStatusIcon *tray=gtk_status_icon_new_from_file("/home/family/Dev/TVAsistant/tv.jpg");
  GtkStatusIcon *tray = gtk_status_icon_new_from_stock(GTK_STOCK_OPEN);
  printf("tray icon: %d", (long)tray);
  //gtk_status_icon_set_from_icon_name(tray,"Test");
  gtk_status_icon_set_tooltip_text(tray,"This is Test");
  gtk_status_icon_set_visible(tray,TRUE);
  
  g_signal_connect(G_OBJECT(tray),"activate",G_CALLBACK(tray_on_click),window);
  g_signal_connect(G_OBJECT(tray),"popup_menu",G_CALLBACK(tray_on_menu),NULL);
  g_signal_connect(G_OBJECT(window),"window_state_event",G_CALLBACK(hide_window),tray);
  */
  
  gtk_widget_show_all(window);
  gtk_main();
}

int appendMessage(const char *msg) {
/*
  if (msgTextBuffer == NULL || msg = NULL)
    return 1;
  
  // Set text
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(msgTextBuffer, &iter);
  //gtk_text_buffer_insert(msgTextBuffer, &iter, msg, -1);
  //gtk_text_buffer_set_text(msgTextBuffer, "1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n01\n2\n3\n4\n5\n6\n7\n8\n9\n0\n", -1);

  // Scroll to bottom
  GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(scrollwindow);
  if (vadj) {
    gtk_adjustment_set_value(vadj, gtk_adjustment_get_upper(vadj));
  }
*/  
  return 0;
}