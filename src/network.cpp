#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <stdio.h>   
#include <errno.h>   
#include <stdlib.h>  
#include <string.h>  
#include <semaphore.h>  
#include <unistd.h>
#include <pthread.h>  
#include <arpa/inet.h>
#include "log.h"
#include "command.h"
#include "network.h"

#define MAXSIZE 512
#define CLIENT_SIZE 10

static pthread_t main_tid; 
static int sock;
static struct sockaddr_in clientAddr[CLIENT_SIZE];
static int clientAddrIndex = -1;

static void *processInstructions(void *arg);
static int authenticate(sockaddr_in* client, char* buffer, int len);

int startServer(const char *host, const char *portStr) {
  int port = atoi(portStr);
  if (port <= 0 || port > 65535) {
    logStderr([portStr](std::ostream &os) { os << "Invalid port number " << portStr << std::endl; } );
    exit(1);
  }
  
  logln("Starting UDP server");
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
//   addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_addr.s_addr = inet_addr(host);
  
  if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
//     logln("Error when creating socket.");
    logStderr([](std::ostream &os) { os << "Error when creating socket." << std::endl; } );
    exit(1);
  }
  if ( bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
//     logln("Error occurred when binding socket on port", port);
    logStderr([host,port](std::ostream &os) { os << "Error occurred when listening on " << host << ":" << port << std::endl; } );
    exit(1);
  } else {
//     log("Socket is listening on", host);
//     logln(" to port", port);
    logStderr([host,port](std::ostream &os) { os << "Socket is listening on " << host << ":" << port << std::endl; } );
  }
    
  if (pthread_create(&main_tid, NULL, processInstructions, NULL)) {
//     logln("Error occurred creating a thread to process network instructions.");
    logStderr([](std::ostream &os) { os << "Error occurred creating a thread to process network instructions." << std::endl; } );
    exit(1);
  }
}

int sendMsgToClient(const char *msg) {
  sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)(clientAddr + clientAddrIndex), sizeof(*clientAddr));
//   log( [msg] (std::ostream &os) { os << "Send UDP packet to client " << clientAddrIndex << ": " << msg << std::endl; } );
}

int sendMsgToClient(const std::function<void(std::ostream&)> &makeMsgCallback) {
  std::ostringstream oss;
  makeMsgCallback(oss);
  std::string msg = oss.str();
  
  sendMsgToClient(msg.c_str());
}

static void *processInstructions(void *arg) {
  logln("Start to process network in a thread.");
  
  char buffer[MAXSIZE];
  int bytes;
  socklen_t addrlen = sizeof(*clientAddr);
  while (1) {
    clientAddrIndex = (clientAddrIndex + 1) % CLIENT_SIZE;
    bytes = recvfrom(sock, buffer, MAXSIZE-1, 0, 
      (struct sockaddr*)(clientAddr + clientAddrIndex), &addrlen);    
    if (bytes>0) {
      buffer[bytes] = 0;
//       log("Data received from", inet_ntoa(clientAddr.sin_addr));   // client IP
//       log(":", ntohs(clientAddr.sin_port));  // client Port
//       logln(", num of bytes:", bytes);
//       logln("Data:", buffer);
      
      if (bytes < 2) {
        logln("Invalid instruction");
      } else {
        int instCode = buffer[0];
        int payloadStartIdx = buffer[1];
        int authSectionLen = payloadStartIdx - 2;
        if (authSectionLen < 0) { 
          authSectionLen = 0; 
        }
        
        if (!authenticate((clientAddr + clientAddrIndex), buffer+2, authSectionLen)) {
          logln("Authentication failed");
          continue;
        } else {
          // Execute the instruction
          execInstruction(instCode, buffer+payloadStartIdx, bytes-payloadStartIdx);
        }
      }
    } else {
      perror("recv");
      break;
    }
  }
}

static int authenticate(sockaddr_in* client, char* buffer, int len) {
//   log( [client] (std::ostream &os) { os << "Request from " << inet_ntoa(client->sin_addr) << std::endl; } );
  if (len <= 0)
    return 1;
  else
    // TO BE COMPLETED
    return 0;
}