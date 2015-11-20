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

