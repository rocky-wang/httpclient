#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <sys/types.h>

int send_http_request(int sockfd,const char *hostname,ushort port,const char *filename);

int recv_http_respond(int sockfd,const char *file);

#endif

