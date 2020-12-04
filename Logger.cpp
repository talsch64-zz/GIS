#include <fstream>
#include <iostream>
#include "Logger.h"

void Logger::error(std::string msg, std::runtime_error ex) {
    std::string fullMsg = msg + " - " + ex.what();
    error(fullMsg);
}

void Logger::error(std::string msg) {
    msg = "ERROR: " + msg + "\n";
    std::ofstream errorLog;
    errorLog.open(ERRORS_FILE_PATH, std::ios_base::app);
    errorLog << msg;
    errorLog.close();
    std::cout << msg << std::endl;
}

void Logger::initialize() {
    std::ofstream ofs;
    ofs.open(ERRORS_FILE_PATH, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}
