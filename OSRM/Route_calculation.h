//
// Created by alexander on 15.04.17.
//

#ifndef BD_CLASS_ROUTE_CALCULATION_H
#define BD_CLASS_ROUTE_CALCULATION_H

#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <cstdlib>

#include <ctime>

using namespace osrm;
using namespace osrm::engine;

// Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
//EngineConfig config;


// Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
//const OSRM osrm_1{config};

class Route_calculation {
public:
    Route_calculation(EngineConfig temp);
    double calculate(std::pair<double, double> start, std::pair<double, double> finish);
    std::vector<double> calculate_from_point_to_vector(std::pair<double, double> start, std::vecotr<std::pair<double, double>> finish);
    std::vector<std::vector<double>> calculate_time_matrix(std::vector<std::pair<double, double>> start, std::vector<std::pair<double, double>> finish);
    std::vector<double> calculate_time_vector(std::vector<std::pair<double, double>> start, std::vector<std::pair<double, double>> finish);
    ~Route_calculation() = default;

private:
    OSRM _osrm;
};


#endif //BD_CLASS_ROUTE_CALCULATION_H

