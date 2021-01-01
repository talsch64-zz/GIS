#ifndef EX1_LOGGER_H
#define EX1_LOGGER_H


#include <string>


/// Logger - Handles errors which arise during the JSON parsing and logs them in "errors.log" file.
/// Uses Exceptions catching to handle the different errors.
/// Each error logged has an indicating message of the error occurred
class Logger {
    const std::string ERRORS_FILE_PATH = "errors.log";

public:
    void initialize();

    void error(std::string msg, std::runtime_error ex);

    void error(std::string msg);
};


#endif //EX1_LOGGER_H
