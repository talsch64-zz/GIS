#ifndef EX1_JSONFILEWRITER_H
#define EX1_JSONFILEWRITER_H

#include <rapidjson/document.h>


/// JsonFileWriter handles JSON file writing
class JsonFileWriter {
public:
    void write(rapidjson::Document &doc, const std::string &filename);
};

#endif //EX1_JSONFILEWRITER_H
