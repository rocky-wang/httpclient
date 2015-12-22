#Design by Rocky

COBJS	+= main.o socket.o parser.o
COBJS	+= http.o downloader.o

CFLAGS	+= -O2 -Wall
CFLAGS	+= -I./inc
CFLAGS	+= -DDEBUG

LDFLAGS	+=

CROSS_COMPILE	?=

CC	= $(CROSS_COMPILE)gcc

TARGET = httpclient

all:$(TARGET)
httpclient:$(COBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
.PHONY:clean
clean:
	rm -f $(COBJS) $(TARGET)
