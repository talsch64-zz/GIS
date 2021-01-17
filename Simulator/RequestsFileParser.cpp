#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include "RequestsFileParser.h"

std::vector<NavigationRequest> RequestsFileParser::parse(const std::string &filePath) const {
    std::ifstream reqStream(filePath);
    std::string line;
    std::vector<NavigationRequest> requests;
    while (std::getline(reqStream, line)) {
        try {
            NavigationRequest req = parseLine(line);
            requests.push_back(req);
        }
        catch (const std::exception &e) {
        }
    }
    if (requests.size() == 0) {
        std::cerr << "ERROR: zero requests given " << std::endl;
        exit(EXIT_FAILURE);
    }
    return requests;
}

NavigationRequest RequestsFileParser::parseLine(const std::string &line) const {
    std::smatch sm;
    const std::regex r(R"(^\{(.+), (.+)\}-\{(.+), (.+)\}$)");
    if (!regex_search(line, sm, r)) {
        throw std::runtime_error("Navigation request line is in incorrect format");
    }
    Coordinates coord[2] = {Coordinates(Longitude(0), Latitude(0)), Coordinates(Longitude(0), Latitude(0))};
    for (std::size_t i = 0; i < 2; i++) {
        std::size_t index = i * 2;
        try {
            double lon = std::stod(sm.str(index + 1));
            double lat = std::stod(sm.str(index + 2));
            coord[i] = Coordinates(Longitude(lon), Latitude(lat));
        }
        catch (const std::invalid_argument &e) {
            throw;
        }
    }
    NavigationRequest req(coord[0], coord[1]);
    return req;
}
