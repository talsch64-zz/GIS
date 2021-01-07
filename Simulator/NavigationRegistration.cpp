#include "../Common/NavigationRegistration.h"
#include "../Common/AbstractGIS.h" // For demo only
#include "../Common/NavigationGIS.h"
#include <iostream> // For demo only

NavigationRegistration::NavigationRegistration(
        std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> factory) {
    // For demo only - You actually want to store the factory function somewere for later use
    // Also, you probably do not want call the factory or create any GIS/NavigationGIS here

    // A demo Way
    struct Way : AbstractWay {
        std::pair<EntityId, EntityId> getJunctions() const {
            return std::make_pair(EntityId{"xyz"}, EntityId{"aaa"});
        }

        Meters getLength() const {
            return Meters{1984};
        }

        bool isBidirectional() const {
            return true;
        }

        int getSpeedLimit() const {
            return 84;
        }

        bool isHighway() const {
            return false;
        }

        bool isToll() const {
            return false;
        }

        std::pair<Meters, Meters> getSegmentPartsOnWay(std::size_t, const Coordinates &) const {
            return std::make_pair(Meters{90}, Meters{10});
        }

        const Coordinates &getFromJunctionCoordinates() const {
            static Coordinates c_from{Longitude{0}, Latitude{0}};
            return c_from;
        }

        const Coordinates &getToJunctionCoordinates() const {
            static Coordinates c_to{Longitude{0}, Latitude{0}};
            return c_to;
        }

        const EntityId &getId() const {
            static EntityId dummy_id{"DUMMY_WAY1000"};
            return dummy_id;
        }
    };

    // A demo GIS
    struct GIS : AbstractGIS {
        std::vector<EntityId> loadMapFile(const std::string &filename) {
            std::cout << "I, GIS_098765432, was requested to open: " << filename
                      << ". I can't do anything with it yet...\n";
            std::vector<EntityId> empty_vector{};
            return empty_vector;
        }

        std::vector<EntityId> getWaysByJunction(const EntityId &) const {
            std::vector<EntityId> returned{EntityId{"W1"}, EntityId{"W2"}};
            return returned;
        }

        std::tuple<Coordinates, EntityId, std::size_t> getWayClosestPoint(const Coordinates &c) const {
            std::tuple<Coordinates, EntityId, std::size_t> returned = std::make_tuple(c, EntityId{"other"},
                                                                                      (std::size_t) 5);
            return returned;
        }

        std::tuple<Coordinates, EntityId, std::size_t>
        getWayClosestPoint(const Coordinates &c, const Restrictions &) const {
            return std::make_tuple(c, EntityId{"other"}, (std::size_t) 5);
        }

        const AbstractWay &getWay(const EntityId &) const {
            static Way dummy_way{};
            return dummy_way;
        }
    } gis;

    NavigationGIS navigation_gis(gis);
    auto navigation = factory(navigation_gis);
    auto routes = navigation->getRoutes({Longitude{0}, Latitude{0}}, {Longitude{180}, Latitude{0}});
    const auto &route = routes->shortestDistance();
    Meters totalLength = route.totalLength();
    std::cout << "Total length of shortest route is: " << static_cast<double>(totalLength) << " Meters\n";
    std::cout << "Usage count of GIS is: " << navigation_gis.getUsageCounter() << '\n';
}
