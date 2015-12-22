#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socketCom.h"
#include "parser.h"
#include "downloader.h"
#include "http.h"
#include "debug.h"

int test_downloader(const char *url,const char *file)
{
	int ret;
	char hostname[64];
	char filename[MAX_URL_FILENAME];
	ushort port;
	int sockfd;

	ret = parser_url(url,hostname,filename,&port);
	if(ret < 0){
		ERROR("parser url error!\n");
		return -1;
	}
	DEBUG_LOG("the hostname is %s\n",hostname);
	DEBUG_LOG("the port is %d\n",port);
	DEBUG_LOG("the filename is %s\n",filename);

	sockfd = create_tcp_client(hostname,port);
	if(sockfd < 0){
		ERROR("tcp client sock failed!\n");
		return -1;
	}

	ret = send_http_request(sockfd,hostname,port,filename);
	if(ret < 0){
		ERROR("send http request error!\n");
		return -1;
	}

	ret = recv_http_respond(sockfd,file);
	if(ret < 0){
		ERROR("recv http request error!\n");
		return -1;
	}

	return 0;
}

int main()
{
	//char buf[] = "http://www.baidu.com:1a80/index.html";
	//char buf[] = "http://west263.newhua.com/down/hfs2_3b285.rar";
	char buf[] = "http://vzidc3.newhua.com/down/PowerWordSetup2016.zip";
	char file[] = "PowerWordSetup2016.zip";
	//char file[] = "index.html";
	//char file[] = "hfs2_3b285.rar";
	int ret;

	ret = test_downloader(buf,file);
	if(ret == -1){
		ERROR("http test downloader failed!\n");
		exit(1);
	}

	INFO("success test downloader!\n");
	return 0;
}

