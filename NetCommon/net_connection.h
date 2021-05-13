#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace bbc
{
	namespace net
	{
		template <typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			connection()
			{}

			virtual ~connection()
			{}

		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

		public:
			bool Send(const message<T>& msg);

		protected:
			// each connection has a unique socket to a remote
			asio::ip::tcp::socket m_socket;

			// this context is shared with the whole asio instance
			asio::io_context& m_asioContext;

			// this queue holds all messages to be sent to the remote side of this connection
			tsqueue<message<T>> m_qMessagesOut;

			// this queue hold all messages that have been recieved from the remote side
			// of this connection. Note it is a reference as the "owner" of this connection
			// is expected to provide a queuee
			tsqueue<owned_message>& m_qMessagesIn;
		};
	}
}