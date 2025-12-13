#pragma once

#include "asio_headers/main.h"
using boost::asio::ip::tcp;

std::string make_daytime_string();

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    using pointer = std::shared_ptr<tcp_connection>;
    static pointer create(boost::asio::io_context& io_context);
    tcp::socket& socket();
    void start();

private:
    explicit tcp_connection(boost::asio::io_context& io_context);
    void handle_write(const boost::system::error_code& /*ec*/, std::size_t /*bytes_transferred*/);

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context);

private:
    void start_accept();
    void handle_accept(tcp_connection::pointer new_connection,
        const boost::system::error_code& error);

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

int daytime3();