//
// Created by student on 12/12/2020.
//

#include "Routes.h"

const Route &Routes::shortestDistance() const {
    return shortestDistanceRoute;
}

const Route &Routes::shortestTime() const {
    return shortestTimeRoute;
}

bool Routes::isValid() const {
    return valid;
}
