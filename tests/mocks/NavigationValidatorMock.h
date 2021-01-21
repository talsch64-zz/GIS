#ifndef GIS_CPP_NAVIGATIONVALIDATORMOCK_H
#define GIS_CPP_NAVIGATIONVALIDATORMOCK_H


#include "../../Simulator/NavigationValidator.h"

class NavigationValidatorMock: public NavigationValidator {
    bool validationResult;
public:
    NavigationValidatorMock(const AbstractGIS &gis, bool validationResult);

    bool validateRoute(const Coordinates &start, const Coordinates &end, const AbstractRoute &routes) const override;
};


#endif //GIS_CPP_NAVIGATIONVALIDATORMOCK_H
