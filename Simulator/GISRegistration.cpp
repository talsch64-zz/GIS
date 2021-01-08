#include "../Common/GISRegistration.h"

GISRegistration::GISRegistration(std::function<std::unique_ptr<AbstractGIS>()> func) {

	func()->loadMapFile("non existant file"); 
}
