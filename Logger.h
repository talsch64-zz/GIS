#ifndef EX1_LOGGER_H
#define EX1_LOGGER_H


#include <string>

class Logger {
    const std::string ERRORS_FILE_PATH = "errors.log";

public:
    void initialize();

    void error(std::string msg, std::runtime_error ex);

    void error(std::string msg);
};


#endif //EX1_LOGGER_H
