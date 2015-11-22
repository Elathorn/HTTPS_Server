#pragma once
#ifndef MHD_PLATFORM_H
#define MHD_PLATFORM_H
#include <winsock2.h> // socket related definitions
#include <ws2tcpip.h> // socket related definitions
#include <sys/types.h>
typedef SSIZE_T ssize_t; //Windows type to unix type
typedef UINT64 uint64_t; //Windows type to unix type
typedef UINT16 uint16_t; //Windows type to unix type
#endif
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     20
#define MAXANSWERSIZE   512

#define GET             0
#define POST            1

#define SERVERKEYFILE "server.key"
#define SERVERCERTFILE "server.pem"