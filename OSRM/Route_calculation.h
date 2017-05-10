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

#include <cstdlib>

#include <ctime>

using namespace osrm;
using namespace osrm::engine;
// EngineConfig config();
// config.storage_config = {"OSRM/RU_MOW.osrm"};
// config.use_shared_memory = false;

// Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
//EngineConfig config;


// Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
//const OSRM osrm_1{config};

class Route_calculation {
public:
    Route_calculation();
    double calculate(std::pair<double, double> start, std::pair<double, double> finish);
    ~Route_calculation() = default;

private:
    OSRM _osrm;
};



#endif //BD_CLASS_ROUTE_CALCULATION_H

