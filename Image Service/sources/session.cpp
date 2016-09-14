#include "session.h"

void Session::doRead() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_data, maxLength), [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec)
            doWrite(length);
    });
}

void Session::doWrite(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(_socket, boost::asio::buffer(_data, length), [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec)
            doRead();
    });
}