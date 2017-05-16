#include "Route_calculation.h"

Route_calculation::Route_calculation(EngineConfig temp):_osrm(temp){

}

double Route_calculation::calculate(std::pair<double, double> start, std::pair<double, double> finish) {
    osrm::RouteParameters params;
    params.coordinates.push_back({osrm::util::FloatLongitude{start.first}, osrm::util::FloatLatitude{start.second}});
    params.coordinates.push_back({osrm::util::FloatLongitude{finish.first}, osrm::util::FloatLatitude{finish.second}});

    // Response is in JSON format
    osrm::json::Object result;
    const auto status = _osrm.Route(params, result);

    if (status == osrm::Status::Ok)
    {
        auto &routes = result.values["routes"].get<osrm::json::Array>();
        auto &route = routes.values.at(0).get<osrm::json::Object>();
        const auto duration = route.values["duration"].get<osrm::json::Number>().value;
        return duration;
    }
    else if (status == osrm::Status::Error)
    {
        return -1;
    }

}

std::vector<double> Route_calculation::calculate_from_point_to_vector(std::pair<double, double> start, std::vector<std::pair<double, double>> finish){
    std::vector<double> result;
    for(size_t i = 0; i < finish.size(); ++i){
        result.push_back(this->calculate(start, finish[i]));
    }
    return result;
}

std::vector<std::vector<double>> Route_calculation::calculate_time_matrix(std::vector<std::pair<double, double>> start, std::vector<std::pair<double, double>> finish){
    std::vector<std::vector<double>> result;
    for(size_t i = 0 ; i < start.size(); ++i){
        result.push_back(this->calculate_from_point_to_vector(start[i], finish));
    }
    return result;
}

std::vector<double> Route_calculation::calculate_time_vector(std::vecotr<std::pair<double, double>> start, std::vecotr<std::pair<double, double>> finish){
    std::vector<double> result;
    for(size_t i = 0 ; i < start.size(); ++i){
        res = calculate_from_point_to_vector(start[i], finish);
        result.push_back(result.end(), res.begin(), res.end());
    }
    return result;
}