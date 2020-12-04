#include <fstream>
#include <iostream>
#include "Logger.h"

void Logger::error(std::string msg, std::exception ex) {
    std::string fullMsg = msg + "\n" + ex.what() + "\n";
    error(fullMsg);
}

void Logger::error(std::string msg) {
    std::ofstream errorLog;
    errorLog.open("errors.log", std::ios_base::app);
    errorLog << msg;
    errorLog.close();
    std::cout << msg << std::endl;
}
