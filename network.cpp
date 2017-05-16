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
    std::vector<std::pair<unsigned, unsigned>> list_drugs;
    rapidjson::Document mess;
    mess.SetObject();
    message_ = std::string(data_, length);
    std::cout << message_ << ": "<< message_.length()<< std::endl;
    if(!mess.Parse(message_.c_str()).HasParseError()){
        lon = mess["Coordinates"]["Longitude"].GetDouble();
        lat = mess["Coordinates"]["Latitude"].GetDouble();
        const rapidjson::Value& list = mess["List"];
        for(size_t i = 0; i < list.Size(); ++i){
            std::pair<unsigned ,unsigned > temp;
            temp.first = list[i]["Id"].GetUint();
            temp.second = list[i]["Count"].GetUint();
            list_drugs.push_back(temp);
        }
        std::cout <<"lon = " << lon << ", lat = " << lat << std::endl;
        for(size_t i = 0; i < list_drugs.size(); ++i){
            std::cout << i << " -   Id: " << list_drugs[i].first << ", count: " << list_drugs[i].second << std::endl;
        }
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