#ifndef EX1_LOGGER_H
#define EX1_LOGGER_H


#include <string>

class Logger {
public:
    void error(std::string msg, std::exception ex);
    void error(std::string msg);
};


#endif //EX1_LOGGER_H
