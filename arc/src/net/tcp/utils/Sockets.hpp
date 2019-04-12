#pragma once

#ifdef _WIN32
	#define NOMINMAX
	#include <WinSock2.h>
	#if !defined(_MSC_VER)
		#include <WS2tcpip.h>
		#define inet_ntop(FAMILY, PTR_STRUCT_SOCKADDR, BUFFER, BUFFER_LENGTH) strncpy(BUFFER, inet_ntoa(*static_cast<struct in_addr*>((PTR_STRUCT_SOCKADDR))), BUFFER_LENGTH)
		#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
    #elif _MSC_VER >= 1800
		#include <WS2tcpip.h>
	#else
		#define inet_ntop(FAMILY, PTR_STRUCT_SOCKADDR, BUFFER, BUFFER_LENGTH) strncpy(BUFFER, inet_ntoa(*static_cast<struct in_addr*>((PTR_STRUCT_SOCKADDR))), BUFFER_LENGTH)
		#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
		using socklen_t = int;
	#endif
	#ifdef _MSC_VER
		#if _WIN32_WINNT == _WIN32_WINNT_WINBLUE
			//!< Win8.1 & higher
			#pragma comment(lib, "Ws2_32.lib")
		#else
			#pragma comment(lib, "wsock32.lib")
		#endif
	#endif
	using nfds_t = unsigned long;
	inline int poll(pollfd fdarray[], nfds_t nfds, int timeout) { return WSAPoll(fdarray, nfds, timeout); }
#else
	#include <sys/socket.h>
	#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
	#include <arpa/inet.h> // hton*, ntoh*, inet_addr
	#include <unistd.h>  // close
	#include <cerrno> // errno
	#include <poll.h> // poll
	#define SOCKET int
	#define INVALID_SOCKET ((int)-1)
	#define SOCKET_ERROR (int(-1))
#endif

#include <string>

namespace Network
{
	bool Start();
	void Release();
	bool SetNonBlocking(SOCKET socket);
	bool SetReuseAddr(SOCKET socket);
	void CloseSocket(SOCKET socket);
	std::string GetAddress(const sockaddr_in& addr);
	unsigned short GetPort(const sockaddr_in& addr);
}
