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
//#include "OSRM/Route_calculation.h"
//#include "osrm/engine_config.hpp"
#include </usr/include/postgresql/libpq-fe.h>

using boost::asio::ip::tcp;

class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, std::string pswd)
        : socket_(std::move(socket)),
          db_pswd(pswd)
        {}

    void start() {
        do_read();
    }

    void stop(){
        socket_.close();
    }


private:
    void do_read();
    void handle(std::size_t length);
    void do_write(std::size_t length);

//    std::string<std::pair<unsigned, unsigned>> pars()

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::string message_;
    std::string db_pswd;
};

#endif //INC_1_NETWORK_H
