#include "server.h"
#include "session.h"

void Server::doAccept() {
	_acceptor.async_accept(_socket, [this](boost::system::error_code ec) {
		if (!ec)
			std::make_shared<Session>(std::move(_socket))->start();

		doAccept();
	});
}