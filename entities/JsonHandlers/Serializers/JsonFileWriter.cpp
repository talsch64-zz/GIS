#include "JsonFileWriter.h"
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>

void JsonFileWriter::write(rapidjson::Document &doc, const std::string &filename) {
    FILE *fp = fopen(filename.c_str(), "wb"); // non-Windows use "w"

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    doc.Accept(writer);

    fclose(fp);
}