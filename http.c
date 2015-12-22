#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "http.h"
#include "socketCom.h"
#include "debug.h"

struct http_request_header* create_http_header()
{
	struct http_request_header*head;

	head = (struct http_request_header*)malloc(sizeof(struct http_request_header));
	if(head == NULL){
		ERROR("create http header failed!\n");
		return NULL;
	}
	head->offset = 0;

	head->request_buf = (char *)malloc(MAX_REQUEST_NUM);
	if(head->request_buf == NULL){
		ERROR("malloc request buf failed!\n");
		return NULL;
	}
	memset(head->request_buf,0,MAX_REQUEST_NUM);

	return head;
}

void release_http_header(struct http_request_header* head)
{
	if(head->request_buf){
		free(head->request_buf);
		head->request_buf = NULL;
	}

	if(head){
		free(head);
	}
}

int add_request(struct http_request_header *head,const char *domain,const char *value)
{
	int i = 0;
	
	if(head == NULL)
		return -1;

	while(domain[i]){
		head->request_buf[head->offset++] = domain[i];
		i++;
	}

	i = 0;
	while(value[i]){
		head->request_buf[head->offset++] = value[i];
		i++;
	}

	head->request_buf[head->offset++] = '\r';
	head->request_buf[head->offset++] = '\n';

	return 0;
}

int add_host_header(struct http_request_header *head,const char *hostname,ushort port)
{
	char buf[128];
	
	if(head == NULL)
		return -1;

	snprintf(buf,128,"%s:%d",hostname,port);

	add_request(head,"Host: ",buf);

	return 0;
}

int init_get_request(struct http_request_header *head,const char *filename)
{
	int i = 0;
	const char *method = "GET ";
	const char *endhead = " HTTP/1.1\r\n";
	
	if(head == NULL){
		return -1;
	}

	head->offset = 0;
	while(method[i]){
		head->request_buf[head->offset++] = method[i];
		i++;
	}

	i = 0;
	while(filename[i]){
		head->request_buf[head->offset++] = filename[i];
		i++;
	}

	i = 0;
	while(endhead[i]){
		head->request_buf[head->offset++] = endhead[i];
		i++;
	}

	return 0;
}

int end_request(struct http_request_header *head)
{
	int i = 0;
	const char *end = "\r\n";
	
	if(head == NULL){
			return -1;
	}

	while(end[i]){
		head->request_buf[head->offset++] = end[i];
		i++;
	}

	head->request_buf[head->offset] = 0;

	return 0;
}

int get_http_status(const char *head)
{
	int i;
	int code = 0;
	int tmp;
	
	if(strncmp(head,"HTTP/1.1",8) != 0){
		ERROR("http respond header not found HTTP/1.1\n");
		return -1;
	}

	head += 9;
	for(i = 0; i < 3; i++){
		tmp = head[i] - '0';
		code = code *10 + tmp;
	}
	
	return code;
}

int get_respond_header(int sockfd,struct http_respond_header *head)
{
	char flags = 1;
	int ret;
	int i = 4;
	char *index = NULL;

	while(i--){
		ret = recv(sockfd,head->respond_buf + head->offset,1024,0);
		if(ret < 0){
			perror("recv respond failed:");
			return -1;
		}
		head->offset += ret;
		if((index = strstr(head->respond_buf,"\r\n\r\n")) != NULL){
			INFO("find respond content index!\n");
			flags = 2;
			break;
		}
	}

	if(flags == 2){
		head->head_part_index = (index - head->respond_buf) + 4;
		head->respond_buf[head->offset] = 0;
		return 0;
	}
	else{
		INFO("not header end flags!\n");
		return -2;
	}
}

struct http_respond_header *create_respond_header()
{
	struct http_respond_header *head_respond;

	head_respond = (struct http_respond_header *)malloc(sizeof(struct http_respond_header));
	if(head_respond == NULL){
		fprintf(stderr,"create respond header failed!\n");
		return NULL;
	}
	head_respond->head_part_index = 0;
	head_respond->offset = 0;

	head_respond->respond_buf = (char *)malloc(MAX_RESPOND_NUM);
	if(head_respond->respond_buf == NULL){
		ERROR("create respond buffer failed!\n");
		return NULL;
	}

	return head_respond;
}

void release_respond_header(struct http_respond_header *head)
{
	if(head->respond_buf){
		free(head->respond_buf);
		head->respond_buf = NULL;
	}

	if(head){
		free(head);
	}
}


