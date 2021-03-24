//
// Created by jorge on 6/15/20.
//

#include "CLogger.hpp"

#include <ctime>
#include <iostream>
#include <iomanip>

void CLogger::printToLog(const char *message){
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout << "[" << std::put_time(&tm, "%D %T" ) << "]: ";
    std::cout << message << std::endl;
}
