#ifndef GIS_CPP_NAVIGATIONTASKMOCK_H
#define GIS_CPP_NAVIGATIONTASKMOCK_H


#include "../../Simulator/NavigationTask.h"

class NavigationTaskMock : public NavigationTask {
    std::unique_ptr<NavigationValidator> mutable validatorMock = nullptr;

public:
    NavigationTaskMock(std::unique_ptr<GISContainer> &gisContainer,
                       std::unique_ptr<NavigationContainer> &navigationContainer, const NavigationRequest &request,
                       int gisIndex, int navigationIndex, int requestIndex);

    const std::unique_ptr<NavigationValidator> &getValidator() const override;
};


#endif //GIS_CPP_NAVIGATIONTASKMOCK_H
