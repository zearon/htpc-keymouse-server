#ifndef _NETWORK_H
#define _NETWORK_H

#include <string>
#include <sstream>
#include <functional>

int startServer(const char *host, const char *portStr);
int stopServer();
int sendMsgToClient(const char *msg);
int sendMsgToClient(const std::function<void(std::ostream&)> &makeMsgCallback);

#endif