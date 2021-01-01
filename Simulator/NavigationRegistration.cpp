#include "../Common/NavigationRegistration.h"

NavigationRegistration::NavigationRegistration(std::function<std::unique_ptr<AbstractNavigation>()> func) {
  // For demo only - You actually want to store this funs somewere for later use
  func()->getRoutes({Longitude{0}, Latitude{0}}, {Longitude{180}, Latitude{0}}); 
}