#pragma once
#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace bbc
{
	namespace net
	{
		template <typename T>
		class client_interface 
		{
			client_interface()
			{
				// intitialise the sockt with the io context, so it can do stuff
			}
			virtual ~client_interface()
			{
				// if the client is destroyed, allways try and disconnect from the server
				Disconnect();
			}

		public:
			// connect to the server with hostname/ip-address and port
			bool Connect(const std::string& host, const uint16_t port)
			{
				try {
					// create connection
					m_connection = std::make_unique<connection<T>>(); // TODO

					// Resolve hostname/ip-address into tangable physical address
					asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					// Tell the connection obj to connect to server
					m_connection->ConnectToServer(m_endpoints);

					// start context thread
					thrContext = std::thread([this]() {m_context.run(); });
				}
				catch(std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}

				return true;
			}

			// disconnect from server
			void Disconnect()
			{
				// if connection exist and its connected then...
				if (IsConnected())
				{
					// disconnect..
					m_connection->Disconnect();
				}

				//either way were also with with the asio context
				m_context.stop();
				// ... and it's thread
				if (thrContext.joinable())
					thrContext.join();

				// destroy the connection obj
				m_connection.release();
			}
			// check if the client is actually connected to a server
			bool IsConnected()
			{
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}

			// retrieve queue of messages from server
			tsqueue<owned_message<T>>& Incoming();
			{
				return m_qMessagesIn;
			}
		protected:
			// asio context handles the data transfer
			asio::io_context m_context;
			// ..but needs a thread of its own to execute it's work commands
			std::thread thrContext;
			// this is the hardware socket that is connected tot he server
			asio::ip::tcp::socket m_socket;
			// the client has a single instance of a "connection" obj, which handles data transfer
			std::unique_ptr<connection<T>> m_connection;


		private:
			// this is the thread safe queue of incoming messages from server
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}