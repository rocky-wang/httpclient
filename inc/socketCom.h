#ifndef SOCKETCOM_H
#define SOCKETCOM_H

extern int create_tcp_client(const char *ip,unsigned short port);

extern int socket_sendnbuf(int sockfd,char *buf,int send_count);

#endif

