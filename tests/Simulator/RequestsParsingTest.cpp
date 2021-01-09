#include <filesystem>
#include "gtest/gtest.h"
#include "../../Simulator/RequestsFileParser.h"

TEST(RequestsParsing, RequestsParsingTest) {
    RequestsFileParser reqParser;
    std::vector<NavigationRequest> expectedReq = {NavigationRequest(Coordinates(Longitude(-12.4443), Latitude(34.44)),
                                                                    Coordinates(Longitude(65.23491),
                                                                                Latitude(31.003))),
                                                  NavigationRequest(Coordinates(Longitude(12.9), Latitude(14)),
                                                                    Coordinates(Longitude(-98.347658),
                                                                                Latitude(-48.1335523)))};

    auto actualReq = reqParser.parse("requests.txt");

    EXPECT_EQ(actualReq.size(), expectedReq.size());
    for (std::size_t i = 0; i < actualReq.size(); i++) {
        EXPECT_EQ(actualReq[i].getFrom(), expectedReq[i].getFrom());
        EXPECT_EQ(actualReq[i].getTo(), expectedReq[i].getTo());
    }
}

TEST(RequestsParsing, BadRequestsParsingTest) {
    RequestsFileParser reqParser;
    std::vector<NavigationRequest> expectedReq = {NavigationRequest(Coordinates(Longitude(54.143), Latitude(38.582)),
                                                                    Coordinates(Longitude(79.53),
                                                                                Latitude(45.132)))};

    auto actualReq = reqParser.parse("requests2.txt");

    EXPECT_EQ(actualReq.size(), expectedReq.size());
    for (std::size_t i = 0; i < actualReq.size(); i++) {
        EXPECT_EQ(actualReq[i].getFrom(), expectedReq[i].getFrom());
        EXPECT_EQ(actualReq[i].getTo(), expectedReq[i].getTo());
    }
}