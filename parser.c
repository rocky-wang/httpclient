#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "debug.h"

static int convert_port(const char *str,ushort *port)
{
	int i = 1;
	ushort sum = 0;
	ushort temp;

	while(str[i]){
		if(str[i] >= '0' && str[i] <= '9'){
			temp = str[i] - '0';
			sum = sum * 10 + temp;
		}
		else{
			ERROR("str port number illegal!\n");
			return -1;
		}
		i++;
	}

	*port = sum;
	return 0;
}

int parser_url(const char *url,char *hostname,char *filename,ushort *port)
{
	char *tmp;
	char *flags;
	int ret;
	
	if(url == NULL){
		ERROR("input url is null!\n");
		return -1;
	}

	tmp = strstr(url,"http://");
	if(tmp == NULL){
		ERROR("url not http:// header!\n");
		return -1;
	}
	url += 7;

	tmp = strstr(url,"/");
	if(tmp == NULL){
		ERROR("url not filename");
		return -1;
	}
	snprintf(hostname,tmp-url+1,"%s",url);
	url = tmp;

	if( (flags = strstr(hostname,":")) != NULL ){
		ret = convert_port(flags,port);
		if(ret == -1){
			ERROR("url assign port error,using default port!\n");
			*port = 80;
		}
		flags[0] = 0;
	}
	else{
		*port = 80;
	}

	snprintf(filename,MAX_URL_FILENAME,"%s",url);
	
	return 0;
}

unsigned int convert_size(const char *head,const char *end)
{
	unsigned int sum = 0;
	int tmp;
	const char *p;
	
	if(head == NULL || end == NULL)
		return 0;

	for(p = head; p < end; p++){
		tmp = *p - '0';
		sum = sum * 10 + tmp;
	}

	return sum;
}

unsigned int get_http_length(const char *respond)
{
	unsigned int size = 0;
	char *head_index;
	char *end;

	head_index = strstr(respond,"Content-Length:");
	if(head_index != NULL){
		head_index += 16;
		end = strstr(head_index,"\r\n");
		size = convert_size(head_index,end);
		return size;
	}

	return 0;
}


