

#ifndef GIS_CPP_NAVIGATIONTASK_H
#define GIS_CPP_NAVIGATIONTASK_H


#include <memory>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "NavigationRequest.h"
#include "GISContainer.h"
#include "NavigationContainer.h"
#include "NavigationValidator.h"
#include "TaskResult.h"

class NavigationTask {
    std::unique_ptr<AbstractNavigation> navigation = nullptr;
    std::unique_ptr<NavigationValidator> validator = nullptr;
    std::unique_ptr<NavigationGIS> navigationGIS = nullptr;
    const NavigationRequest &request;
    int gisIndex;
    int navigationIndex;
    int requestIndex;

protected:
    std::unique_ptr<GISContainer> &gisContainer;

public:
    NavigationTask(std::unique_ptr<GISContainer> &gisContainer,
                   std::unique_ptr<NavigationContainer> &navigationContainer, const NavigationRequest &request,
                   int gisIndex, int navigationIndex, int requestIndex);

    std::unique_ptr<TaskResult> execute();

    int getGisIndex() const;

    int getNavigationIndex() const;

    int getRequestIndex() const;

    GISContainer &getGisContainer() const;
};


#endif //GIS_CPP_NAVIGATIONTASK_H
