#include <iostream>

#ifdef _WIN32
	#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				GrabSomeData(socket);
				std::cout << "\n\nRead " << length << " bytes\n\n";
				for (int i = 0; i < length; i++)
					std::cout << vBuffer[i];

				
			}
		}
	);
}

int main()
{
	asio::error_code ec;

	// Create a "context" - essential the platform specific interface
	asio::io_context context;

	// give some fake work to the context so it doesnt finish
	asio::io_context::work idleWork(context);

	// start the context
	std::thread thrContext = std::thread([&]() {context.run(); });

	// Get the address of somewhere we wish to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80); // 127.0.0.1

	// create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	// tell the socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address: \n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		GrabSomeData(socket);
	}

	system("pause");
	return 0;
}