#pragma once
#include <vector>

namespace Network
{
	namespace Messages
	{
#define DECLARE_MESSAGE(name) friend class Base; static const Base::Type StaticType = Base::Type::name
		class Base
		{
			public:
				template<class M>
				bool is() const { return mType == M::StaticType; }
				template<class M>
				const M* as() const { return static_cast<const M*>(this); }

				sockaddr_in from;
				uint64_t idFrom;

			protected:
				enum class Type {
					Connection,
					Disconnection,
					UserData,
				};
				Base(Type type)
					: mType(type)
				{}
			private:
				Type mType;
		};
		class Connection : public Base
		{
			DECLARE_MESSAGE(Connection);
			public:
				enum class Result {
					Success,
					Failed,
				};
				Connection(Result r)
					: Base(Type::Connection)
					, result(r)
				{}
				Result result;
		};
		class Disconnection : public Base
		{
			DECLARE_MESSAGE(Disconnection);
			public:
				enum class Reason {
					Disconnected,
					Lost,
				};
				Disconnection(Reason r)
					: Base(Type::Disconnection)
					, reason(r)
				{}
				Reason reason;
		};
		class UserData : public Base
		{
			DECLARE_MESSAGE(UserData);
			public:
				UserData(std::vector<unsigned char>&& d)
					: Base(Type::UserData)
					, data(std::move(d))
				{}
				std::vector<unsigned char> data;
		};
#undef DECLARE_MESSAGE
	}
}