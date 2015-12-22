#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG

//blue
#define DEBUG_LOG(arg...)	do{fprintf(stdout,"\033[0;34;40m");\
	fprintf(stdout,"[Debug:%s in %s function at %d line: ]",__FILE__,__FUNCTION__,__LINE__ - 1);\
	fprintf(stdout,arg); fprintf(stdout,"\033[0m");\
	fflush(stdout);}while(0);

//green
#define INFO(arg...)	do{fprintf(stdout,"\033[0;36;40m");\
	fprintf(stdout,"[Info:%s in %s function at %d line: ]\n",__FILE__,__FUNCTION__,__LINE__ - 1);\
	fprintf(stdout,arg); fprintf(stdout,"\033[0m");\
	fflush(stdout);}while(0);

//red
#define ERROR(arg...)	do{fprintf(stderr,"\033[0;31;40m");\
	fprintf(stderr,"[error:%s in %s function at %d line: ]",__FILE__,__FUNCTION__,__LINE__ - 1);\
	fprintf(stderr,arg); fprintf(stderr,"\033[0m");}while(0);

#else

#define DEBUG_LOG(arg...) 

#define INFO(arg...)

#define ERROR(arg...) do{fprintf(stderr,"\033[0;31;40m");\
	fprintf(stderr,"[error]:");\
	fprintf(stderr,arg); fprintf(stderr,"\033[0m");}while(0);

#endif

#endif

