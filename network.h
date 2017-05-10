//
// Created by alexander on 03.05.17.
//

#ifndef INC_1_NETWORK_H
#define INC_1_NETWORK_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <boost/asio.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using boost::asio::ip::tcp;

class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read();
    void handle(std::size_t length);
    void do_write(std::size_t length);

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::string message_;
};

#endif //INC_1_NETWORK_H
