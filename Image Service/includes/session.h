#include <boost/asio.hpp>

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket) :
        _socket(std::move(socket)) {

        }

    void start() { doRead(); }

private:
    void doRead();
    void doWrite(std::size_t length);
    boost::asio::ip::tcp::socket _socket;
    enum { maxLength = 1024 };
    char _data[maxLength];
};
