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

#define MAXSIZE 512

static pthread_t main_tid; 
static int sock;
static struct sockaddr_in clientAddr;

static void *processInstructions(void *arg);
static int authenticate(char* buffer, int len);

int startServer(int port) {
  logln("Starting UDP server");
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    logln("Error when creating socket.");
    exit(1);
  }
  if ( bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
    logln("Error occurred when binding socket on port", port);
    exit(1);
  } else {
    logln("Socket is bound to port", port);
  }
    
  if (pthread_create(&main_tid, NULL, processInstructions, NULL)) {
    logln("Error occurred creating a thread to process network instructions.");
    exit(1);
  }
}

int sendMsgToClient(const char *msg) {
  sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
}

static void *processInstructions(void *arg) {
  logln("Start to process network in a thread.");
  
  char buffer[MAXSIZE];
  int bytes;
  socklen_t addrlen = sizeof(clientAddr);
  while (1) {
    bytes = recvfrom(sock, buffer, MAXSIZE-1, 0, (struct sockaddr*)&clientAddr, &addrlen);    
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
        
        if (!authenticate(buffer+2, authSectionLen)) {
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

static int authenticate(char* buffer, int len) {
  if (len <= 0)
    return 1;
  else
    // TO BE COMPLETED
    return 0;
}