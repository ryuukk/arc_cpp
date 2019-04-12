#pragma once

#pragma once

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <cerrno>
	#define SOCKET int
	#define INVALID_SOCKET ((int)-1)
	#define SOCKET_ERROR (int(-1))
#endif

namespace Network
{
	namespace Errors
	{
		int Get();
		enum {
#ifdef _WIN32
			AGAIN = WSATRY_AGAIN,
			WOULDBLOCK = WSAEWOULDBLOCK,
			INPROGRESS = WSAEINPROGRESS,
			INTR = WSAEINTR,
#else
			AGAIN = EAGAIN,
			WOULDBLOCK = EWOULDBLOCK,
			INPROGRESS = EINPROGRESS,
			INTR = EINTR,
#endif
		};
	}
}