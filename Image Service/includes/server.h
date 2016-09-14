#include <boost/asio.hpp>

class Server {
public:
	Server(boost::asio::io_service& ioService, short port) :
		_acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		_socket(ioService) {
			doAccept();
		}

private:
	void doAccept();

	boost::asio::ip::tcp::acceptor _acceptor;
	boost::asio::ip::tcp::socket _socket;
};
