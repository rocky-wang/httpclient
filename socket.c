#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "debug.h"

int create_tcp_client(const char *ip,unsigned short port)
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *tmp;
	int ret;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket");
		return -1;
	}
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family 	= AF_INET;
	server_addr.sin_port	= htons(port);
	tmp = gethostbyname(ip);
	if(tmp == NULL){
		perror("gethostbyname");
		return -1;
	}
	memcpy(&server_addr.sin_addr,(struct in_addr *)tmp->h_addr,4);
	
	ret = connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret == -1){
		perror("connect");
		return -1;
	}
	
	INFO("TCP connect %s success!\n",ip);
	
	return sockfd;
}

int socket_sendnbuf(int sockfd,char *buf,int send_count)
{
	int ret;
	int slen = 0;

	while(slen != send_count){
		ret = send(sockfd,buf+slen,send_count -slen,0);
		if(ret < 0){
			perror("send socket");
			return -1;
		}
		slen += ret;
	}

	DEBUG_LOG("send %d bytes success!\n",slen);

	return 0;
}

