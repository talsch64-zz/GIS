#include "GISContainer.h"

GISContainer::GISContainer(const std::function<std::unique_ptr<AbstractGIS>()> &factory, const std::string &name)
        : factory(factory), name(name) {}
