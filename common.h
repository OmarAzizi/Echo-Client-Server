#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <syscall.h>

#define EXIT_SUCCESS   0
#define EXIT_FALIURE   1

#define SERV_PORT     8080
#define MAXLINE       4096
#define SA struct sockaddr
#define LISTENQ       1024
#define MAX_BACKLOG    100

void handle_errors(const char*, ...);
char* bin2hex(const unsigned char*, size_t);
void Listen(int, int);

#endif
