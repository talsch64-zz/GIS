#include "gtest/gtest.h"
#include <fstream>
#include <cstring>


class SimulatorTest : public ::testing::Test {

public:
    SimulatorTest() {
    }

    void SetUp() override {
    }

    void TearDown() override {

    }

    std::vector<std::string> getStrangeResultsLines() {
        std::vector<std::string> lines;
        std::ifstream infile("strange_GIS_results.log");
        std::string line;
        while (std::getline(infile, line)) {
            lines.push_back(line);
        }
        lines.pop_back();
        return lines;
    }

    std::vector<std::string> splitLineToParts(std::string line) {
        std::vector<std::string> arr;
        char *cstr = const_cast<char *>(line.c_str());
        std::string sep = ", ";
        char *current = std::strtok(cstr, sep.c_str());
        while (current != nullptr) {
            arr.emplace_back(current);
            current = std::strtok(nullptr, sep.c_str());
        }
        return arr;
    }

};


TEST_F(SimulatorTest, getSegmentPartsOnWayTest1) {
}