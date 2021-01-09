#include "NavigationRequest.h"

NavigationRequest::NavigationRequest(const Coordinates &from, const Coordinates &to) : from(from), to(to) {}

const Coordinates &NavigationRequest::getFrom() const {
    return from;
}

const Coordinates &NavigationRequest::getTo() const {
    return to;
}
