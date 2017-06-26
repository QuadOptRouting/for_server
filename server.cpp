//
// Created by alexander on 02.05.17.
//

#include "server.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

server::server(boost::asio::io_service& io_service, short port, std::string pswd):
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service),
    db_pswd(pswd) {
    do_accept();
}

void server::do_accept() {
    acceptor_.async_accept(socket_,
                           [this](boost::system::error_code ec)
                           {
                               if (!ec)
                               {
                                   std::make_shared<session>(std::move(socket_), db_pswd)->start();
                               }

                               do_accept();
                           });
}