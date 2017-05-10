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