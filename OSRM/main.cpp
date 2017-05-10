/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: alexander
 *
 * Created on 18 апреля 2017 г., 16:10
 */

#include <cstdlib>
#include "Route_calculation.h"
#include <utility>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Route_calculation ex();
    double result;
    std::pair<double, double> start(37.414743, 55.737849), finish(37.812997, 55.807517);
    result = ex.claculate(start, finish);
    std::cout << "result = " << result << std::endl;
    return 0;
}

