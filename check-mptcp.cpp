#include <iostream>

#include <boost/asio.hpp>

#include <sys/types.h>
#include <sys/socket.h>

namespace asio = boost::asio;

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <domain> <port>\n";
		return 2;
	}
	asio::io_context context;

	boost::system::error_code ec;

	asio::ip::tcp::socket socket{context};
	asio::ip::tcp::resolver resolver{context};

	auto results = resolver.resolve(argv[1], argv[2], ec);
	if (ec != boost::system::errc::success) {
		std::cerr << "Error: " << ec << std::endl;
		return 1;
	}

	for (auto result : results) {
		socket.connect(result.endpoint(), ec);
		if (ec == boost::system::errc::success) {
			std::cout << "Connected: "
				  << socket.remote_endpoint().address() << ' '
				  << socket.remote_endpoint().port() << '\n';
			break;
		}
	}

	const int MPTCP_ENABLED = 42;
	int fd = socket.native_handle();
	int enabled = 0;
	socklen_t enabled_len = sizeof(enabled);
	int flags = getsockopt(fd, SOL_TCP, MPTCP_ENABLED, &enabled, &enabled_len);

	std::cout << "flags: " << flags << std::endl;
	std::cout << "enabled: " << enabled << std::endl;
	return 0;
}
