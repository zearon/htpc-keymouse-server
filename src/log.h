#ifndef _LOG_H
#define _LOG_H

#include <functional>
#include <sstream>
#include <string>

/**
 * Accept a lambda expression callback void(std::ostringstream) as argument in which
 * the ostringstream is the destination of log
 */
void log(const std::function<void(std::ostream&)> &callback);

void log(const char* str);
void log(double num);
void log(const char* str1, const char* str2);
void log(const char* str1, double num1);
void logln(const char* str);
void logln(const char* str1, const char* str2);
void logln(const char* str1, double num1);

void logStderr(const std::function<void(std::ostream&)> &callback);

#endif