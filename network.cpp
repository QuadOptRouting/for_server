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
    double lon, lat;
    rapidjson::Document mess;
    mess.SetObject();
    message_ = std::string(data_, length);
    std::cout << message_ << ": "<< message_.length()<< std::endl;
    if(!mess.Parse(message_.c_str()).HasParseError()){
        lon = mess["lon"].GetDouble();
        lat = mess["lat"].GetDouble();
        std::cout <<"lon = " << lon << ", lat = " << lat << std::endl;
    }
    osrm::engine::EngineConfig temp;
    temp.storage_config = {"for_server/OSRM/osrm/RU-MOW.osrm"};
    temp.use_shared_memory = false;
    Route_calculation ex(temp);
    double result;
    std::pair<double, double> start(lon, lat), finish(37.812997, 55.807517);
    result = ex.calculate(start, finish);
    std::cout << "time: "<<result << std::endl;
    message_ = std::to_string(result);
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