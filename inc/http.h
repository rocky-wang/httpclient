#ifndef HTTP_H
#define HTTP_H

#include <sys/types.h>

struct http_request_header{
	char *request_buf;
	int offset;
};

#define MAX_REQUEST_NUM  4096

struct http_respond_header{
	char *respond_buf;
	int offset;
	int head_part_index;
};

#define MAX_RESPOND_NUM 4096

struct http_request_header* create_http_header();

int init_get_request(struct http_request_header *head,const char *filename);

int add_request(struct http_request_header *head,const char *domain,const char *value);

int add_host_header(struct http_request_header *head,const char *hostname,ushort port);
int end_request(struct http_request_header *head);

struct http_respond_header *create_respond_header();

void release_respond_header(struct http_respond_header *head);

void release_http_header(struct http_request_header* head);

int get_http_status(const char *head);

int get_respond_header(int sockfd,struct http_respond_header *head);

#endif
