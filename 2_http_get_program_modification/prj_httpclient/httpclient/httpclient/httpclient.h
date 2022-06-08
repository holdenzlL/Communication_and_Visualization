#ifndef _HEADER_HTTPCLIENT_
#define _HEADER_HTTPCLIENT_
//Socket initialization
/*
The first part includes winsock.h and ws2tcpip.h on Windows. _WIN32_WINNT must be
defined for the Winsock headers to provide all the functions we need. We also include
the #pragma comment(lib,"ws2_32.lib") pragma statement. This tells the Microsoft
Visual C compiler to link your program against the Winsock library, ws2_32.lib. If you're
using MinGW as your compiler, then #pragma is ignored. In this case, you need to tell the
compiler to link in ws2_32.lib on the command line using the -lws2_32 option.
*/
#if defined(_WIN32)
#ifndef _WIN32_WINNT	// specify the minimum operating system version your code supports.
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

/*
On Windows, socket() returns INVALID_SOCKET if it fails. 
On Unix, socket() returns a negative number on failure.
*/
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#endif


/*
in UNIX, all file descriptors (and therefore socket descriptors) are small, nonnegative
integers. In Windows, a socket handle can be anything. Furthermore, in UNIX, the
socket() function returns an int, whereas in Windows it returns a SOCKET. SOCKET is a
typedef for an unsigned int in the Winsock headers. As a workaround, I find it useful
to either typedef int SOCKET or #define SOCKET int on non-Windows platforms.
That way, you can store a socket descriptor as a SOCKET type on all platforms:
*/
#if !defined(_WIN32)
#define SOCKET int
#endif

/*
All sockets on Unix systems are also standard file descriptors. For this reason, sockets on
Unix systems can be closed using the standard close() function. On Windows, a special
close function is used instead¡ªclosesocket().
*/
#if defined(_WIN32)
#define CLOSESOCKET(s) closesocket(s)
#else
#define CLOSESOCKET(s) close(s)
#endif

/*
When a socket function, such as socket(), bind(), accept(), and so on, has an error on
a Unix platform, the error number gets stored in the thread-global errno variable. On
Windows, the error number can be retrieved by calling WSAGetLastError() instead.
*/
#if defined(_WIN32)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define GETSOCKETERRNO() (errno)
#endif

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "str.h"
#include "url.h"

#define _DEBUG_MODE				//open debug mode for printing more information
#define _MAX_SEND_BUFFER_SIZE 2048
#define _MAX_RECV_BUFFER_SIZE 30000
#define _MAX_WAITING_TIME_TOTAL	-1 // set it to be -1 for ultimate waiting time
#define _MAX_WAITING_TIME_PER_PACKET 20

typedef struct 
{
	FILE* fp;
	size_t size;
	size_t headerSize;
	size_t bodySize;
	fpos_t headerPosition;
	fpos_t bodyPosition;
	str_p protocol;
	int stateCode;
	bool ifChunked;	//if the data is chunked, then true, otherwise false
	long contentLength;
}datapack;

void ALLinitialization();								// Initialization
void send_request(SOCKET _socket, url _url);			// Fabricate and send request
SOCKET connect2server(url _url);						// create socket and connect to server
void ALLcleanup(datapack data, SOCKET _socket);			// call this function to do all cleaning work
datapack recv_response(SOCKET _socket);					// Receive response from the server
void headerExtractor(datapack* data);					// Extract header info
void postProcessor(datapack* data, FILE* downloadpath);	// Extract body info

void perr_exit(const char* msg, int ret_code);			// Error Handling

#endif /* _HEADER_HTTPCLIENT_ */