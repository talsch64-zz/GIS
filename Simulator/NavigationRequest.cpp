#include "NavigationRequest.h"
#include <string>

NavigationRequest::NavigationRequest(const Coordinates &from, const Coordinates &to) : from(from), to(to) {}

const Coordinates &NavigationRequest::getFrom() const {
    return from;
}

const Coordinates &NavigationRequest::getTo() const {
    return to;
}

std::string NavigationRequest::toString() const {
    std::string str =
            "{" + std::to_string((double) from.longitude()) + ", " + std::to_string((double) from.latitude()) + "}-{" +
            std::to_string((double) to.longitude()) + ", " +
            std::to_string((double) to.latitude()) + "}";
    return str;
}
