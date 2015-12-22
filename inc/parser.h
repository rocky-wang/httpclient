#ifndef PARSER_H
#define PARSER_H
#include <sys/types.h>

#define MAX_URL_FILENAME  256
extern int parser_url(const char *url,char *hostname,char *filename,ushort *port);

extern unsigned int get_http_length(const char *respond);

#endif