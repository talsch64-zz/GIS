#pragma once 

#include "AbstractNavigation.h"
#include <memory> 
#include <functional>

class NavigationRegistration {
public:
  NavigationRegistration(std::function<std::unique_ptr<AbstractNavigation>()>);
};

#define REGISTER_NAVIGATION(class_name) \
NavigationRegistration register_me_##class_name \
	([]{return std::make_unique<class_name>();} );

