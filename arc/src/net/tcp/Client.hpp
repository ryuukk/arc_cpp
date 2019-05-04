#pragma once

#include "utils/Sockets.hpp"
#include <string>
#include <memory>

namespace Network
{
	namespace Messages {
		class Base;
	}
	namespace TCP
	{
		using HeaderType = uint16_t;
		static const unsigned int HeaderSize = sizeof(HeaderType);
		class ClientImpl;
		class Client
		{
			public:
				Client();
				Client(const Client&) = delete;
				Client& operator=(const Client&) = delete;
				Client(Client&&);
				Client& operator=(Client&&);
				~Client();

				bool init(SOCKET&& sckt, const sockaddr_in& addr);
				bool connect(const std::string& ipaddress, unsigned short port);
				void disconnect();
				bool send(const unsigned char* data, unsigned int len);
				std::unique_ptr<Messages::Base> poll();

				uint64_t id() const;
				const sockaddr_in& destinationAddress() const;

			private:
				std::unique_ptr<ClientImpl> mImpl;
		};
	}
}