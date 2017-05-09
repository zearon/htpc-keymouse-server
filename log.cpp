#include <fstream>
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

void log(const char* str) {
  init(); 
  *file <<str;
  file->flush();
}

void log(double num) {
  init(); 
  *file <<num;
  file->flush();
}

void log(const char* str1, const char* str2) {
  init();
  *file <<str1 <<" " <<str2;
  file->flush();
}

void log(const char* str1, double num1) {
  init();
  *file <<str1 <<" " <<num1;
  file->flush();
}

void logln(const char* str) {
  init();
  *file <<str <<endl;
  file->flush();
}

void logln(const char* str1, const char* str2) {
  init();
  *file <<str1 <<" " <<str2 <<endl;
  file->flush();
}

void logln(const char* str1, double num1) {
  init();
  *file <<str1 <<" " <<num1 <<endl;
  file->flush();
}