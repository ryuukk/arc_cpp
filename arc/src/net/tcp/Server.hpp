#pragma once

#include <memory>

namespace Network
{
	namespace Messages {
		class Base;
	}
	namespace TCP
	{
		class ServerImpl;
		class Server
		{
			public:
				Server();
				Server(const Server&) = delete;
				Server& operator=(const Server&) = delete;
				Server(Server&&);
				Server& operator=(Server&&);
				~Server();

				bool start(unsigned short _port);
				void stop();
				void update();
				bool sendTo(uint64_t clientid, const unsigned char* data, unsigned int len);
				bool sendToAll(const unsigned char* data, unsigned int len);
				std::unique_ptr<Messages::Base> poll();

			private:
				std::unique_ptr<ServerImpl> mImpl;
		};
	}
}