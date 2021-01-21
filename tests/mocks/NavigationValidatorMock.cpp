#include "NavigationValidatorMock.h"

NavigationValidatorMock::NavigationValidatorMock(const AbstractGIS &gis, bool validationResult) : NavigationValidator(
        gis), validationResult(validationResult) {}

bool NavigationValidatorMock::validateRoute(const Coordinates &start, const Coordinates &end,
                                            const AbstractRoute &routes) const {
    NavigationValidator::validateRoute(start, end, routes);
    return validationResult;
}
