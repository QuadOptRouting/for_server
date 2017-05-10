//
// Created by alexander on 03.05.17.
//

#include "network.h"

void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    handle(length);
                                }
                            });
}

void session::handle(std::size_t length){
    message_ = std::string(data_, length) + std::string(data_, length);
    std::cout << length << ": "<< message_.length()<< std::endl;
    do_write(length);
}

void session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(message_, message_.length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     do_read();
                                 }
                             });
}