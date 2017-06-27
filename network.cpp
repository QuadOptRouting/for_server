//
// Created by alexander on 03.05.17.
//

#include "network.h"

std::vector<std::pair<int, std::string>> findPossibleDrugs(std::string query, std::string pswd)
{
    /* executes
        SELECT * FROM "QOR_main_scheme".drug
        WHERE drug.name LIKE '%query%';
    */
    std::shared_ptr<PGconn> connection;
    connection.reset( PQsetdbLogin( "185.195.24.240",
                      "5432",
                      nullptr, nullptr,
                      "main",
                      "postgres",
                      PQencryptPassword( pswd.c_str(),
                      "postgres" )),
                      &PQfinish );
    if ( PQstatus( connection.get() ) != CONNECTION_OK )
        throw std::runtime_error( PQerrorMessage( connection.get() ) );
    else {
        std::string request = "SELECT * FROM \"QOR_main_scheme\".drug WHERE drug.name LIKE \'\%" + query + "\%\' LIMIT 20;";
        PGresult* temp = PQexec( connection.get(), request.c_str() );
        std::vector<std::pair<int, std::string>> result;
        for(int i = 0; i < PQntuples(temp); ++i)
        {
            int id = std::stoi(PQgetvalue(temp, i, 0));
            std::string name = PQgetvalue(temp, i, 1);
            result.push_back(std::pair<int, std::string>(id, name));
        }
        return result;
    }
}

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
    bool sigStart = false; // signal for algorithm start
    std::vector<std::pair<int, std::string>> possibleDrugs; // possible pairs id-name for current query

    message_ = std::string(data_, length);
    std::cout << "Reuest: " << message_ << std::endl;

    rapidjson::Document mess;
    mess.SetObject();
    if(!mess.Parse(message_.c_str()).HasParseError()) {
        if( mess.HasMember("Coordinates") ) {
            lon = mess["Coordinates"]["Longitude"].GetDouble();
            lat = mess["Coordinates"]["Latitude"].GetDouble();
        }
        if( mess.HasMember("List") ) {
            const rapidjson::Value& list = mess["List"];
            for(size_t i = 0; i < list.Size(); ++i){
                std::pair<unsigned ,unsigned > temp;
                temp.first = list[i]["Id"].GetUint();
                temp.second = list[i]["Count"].GetUint();
                list_drugs.push_back(temp);
            }
        }
        if( mess.HasMember("Signal") ){
            sigStart = (bool)mess["Signal"].GetUint();
        }
        if( mess.HasMember("Query") ){
            std::string query = mess["Query"].GetString();
            if(query.length() > 1)
                // do not search for very short strings
                possibleDrugs = findPossibleDrugs(query, db_pswd);
        }

        // assume that there is either sigStart OR some query to send back
        if( !sigStart ) {
            // prepare json with query result
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            writer.StartObject();
            writer.Key("Variants");
                writer.StartArray();
                for(auto it : possibleDrugs)
                {
                    writer.StartObject();
                    writer.Key("ID");
                    writer.Uint(it.first);
                    writer.Key("Name");
                    writer.String(it.second.c_str());
                    writer.EndObject();
                }
            writer.EndArray();
            writer.EndObject();
            message_ = buffer.GetString();
        } else {
            // start algorithm

        }
    } else {
        /* exceptions */
    }

    std::cout << "Response: " << message_ << std::endl;
/*
    osrm::engine::EngineConfig temp;

    temp.storage_config = {"../OSRM/osrm/RU-MOW.osrm"};
    temp.use_shared_memory = false;
    osrm::OSRM osrm_t(temp);
    Route_calculation ex(&osrm_t);
    double result;
    std::pair<double, double> start(lon, lat), finish(37.812997, 55.807517);
    result = ex.calculate(start, finish);


    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    writer.StartObject();
    writer.Key("Time");
    writer.Double(result);
    writer.EndObject();


    std::cout << "time: "<<result << std::endl;
    message_ = buf.GetString();
    std::cout << message_ << std::endl;
*/
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