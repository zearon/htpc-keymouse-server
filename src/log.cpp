#include <fstream>
#include <iostream>
#include "log.h"
#include "gui.h"

using namespace std;

static const char *filename = "./log.txt";
static ofstream *file = NULL;

static void init() {
  if (file == NULL) {
    file = new ofstream();
    file->open(filename, ios::app); 
  }
}

static void _internal_log(const std::function<void(std::ostream&)> &callback) {
  init(); 
  callback(*file);
  file->flush();
}

void log(const std::function<void(std::ostream&)> &callback) {
  ostringstream oss;
  callback(oss);
  _internal_log( [&] (ostream &os) { os << oss.str(); } );
}

void log(const char* str) {
  _internal_log( [=] (ostream &os) { os << str; } );
}

void logln(const char* str) {
  _internal_log( [=] (ostream &os) { os << str << endl; } );
}

void log(double num) {
  _internal_log( [=] (ostream &os) { os << num; } );
}

void log(const char* str1, const char* str2) {
  _internal_log( [=] (ostream &os) { os <<str1 <<" " <<str2; } );
}

void logln(const char* str1, const char* str2) {
  _internal_log( [=] (ostream &os) { os <<str1 <<" " <<str2 <<endl; } );
}

void log(const char* str1, double num1) {
  _internal_log( [=] (ostream &os) { os <<str1 <<" " <<num1; } );
}

void logln(const char* str1, double num1) {
  _internal_log( [=] (ostream &os) { os <<str1 <<" " <<num1 <<endl; } );
}

void logStderr(const std::function<void(std::ostream&)> &callback) {
  ostringstream oss;
  callback(oss);
  cerr << oss.str();
  _internal_log( [&] (ostream &os) { os << oss.str(); } );
}