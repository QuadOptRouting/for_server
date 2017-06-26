//
// Created by alexander on 02.05.17.
//

#ifndef INC_1_SERVER_H
#define INC_1_SERVER_H


#include "network.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <functional>


using boost::asio::ip::tcp;

class server
{
public:
    server(boost::asio::io_service& io_service, short port, std::string pswd);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::string db_pswd;
};


#endif //INC_1_SERVER_H
