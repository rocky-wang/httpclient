#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "http.h"
#include "parser.h"
#include "socketCom.h"
#include "debug.h"

int send_http_request(int sockfd,const char *hostname,ushort port,const char *filename)
{
	struct http_request_header *request;
	int ret;

	request = create_http_header();

	init_get_request(request,filename);

	add_host_header(request,hostname,port);
	add_request(request,"Accept: ","*/*");
	//add_request(request,"Accept-Language: ","zh-cn");
	//add_request(request,"User-Agent: ","Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
	//add_request(request,"Refer: ","http://www.onlinedown.net/softdown/28017_2.htm");
	add_request(request,"Connection: ","Close");

	end_request(request);

	INFO("%s\n",request->request_buf);
	INFO("request len is %d\n",request->offset);

	ret = socket_sendnbuf(sockfd,request->request_buf,request->offset);
	if(ret < 0){
		ERROR("send http request error!\n");
		close(sockfd);
		release_http_header(request);
		return -1;
	}

	INFO("send http request success!\n");
	release_http_header(request);

	return 0;
}

FILE *init_tofile(const char *file,struct http_respond_header *head)
{
	FILE *fp;
	int ret;

	fp = fopen(file,"w");
	if(fp == NULL){
		perror("fopen error");
		return NULL;
	}

	ret = fwrite(head->respond_buf+head->head_part_index,sizeof(char),
			head->offset - head->head_part_index,fp);
	if(ret < 0){
		perror("init fwrite");
		fclose(fp);
		return NULL;
	}

	return fp;
}

int download_file(int sockfd,FILE *fp,unsigned int size,unsigned int downed)
{
	char buf[1024];
	int ret;
	int w_ret;
	double per;
	int freq = 0;

	per = (downed*100.0) / size;

	//printf("\rdowned:%u --- percent: %%%f",downed,per);
	ret = recv(sockfd,buf,sizeof(buf),0);
	while(ret){
		w_ret = fwrite(buf,sizeof(char),ret,fp);
		if(w_ret < 0){
			perror("fwrite");
			return -1;
		}
		downed += w_ret;
		if(freq >= 10){
			per = (downed*100.0) / size;
			printf("\rdowned:%u --- percent: %%%f",downed,per);
			freq = 0;
		}

		ret = recv(sockfd,buf,sizeof(buf),0);
		if(ret < 0){
			perror("recv http respond failed:");
			fclose(fp);
			return -1;
		}
		freq++;
	}

	fclose(fp);
	close(sockfd);

	return 0;
}

int recv_http_respond(int sockfd,const char *file)
{
	struct http_respond_header *head;
	int ret;
	int code;
	unsigned int size;
	unsigned int downed;
	FILE *fp;

	head = create_respond_header();
	if(head == NULL){
		ERROR("create respond header error!\n");
		return -1;
	}

	ret = get_respond_header(sockfd,head);
	if(ret < 0){
		ERROR("Not Found respond header!\n");
		return -1;
	}
	DEBUG_LOG("the respond is\n");
	INFO("%s\n",head->respond_buf);

	code = get_http_status(head->respond_buf);
	INFO("the respond http status is %d\n",code);
	if(code != 200){
		ERROR("Not support the code!\n");
		return -1;
	}

	downed = head->offset - head->head_part_index;

	size = get_http_length(head->respond_buf);
	INFO("http return length is %u\n",size);

	fp = init_tofile(file,head);
	if(fp == NULL){
		ERROR("init to file failed!\n");
		return -1;
	}
	DEBUG_LOG("ready free respond space!\n");
	release_respond_header(head);

	ret = download_file(sockfd,fp,size,downed);
	if(ret < 0){
		ERROR("download file failed!\n");
		return -1;
	}

	return 0;
}

