#include "AircraftCarrier.h"

namespace acg {

    int AircraftCarrier::getMaxAircraftCapacity() const {
        return max_aircraft_capacity;
    }

    void AircraftCarrier::setMaxAircraftCapacity(int max_cap) {
        max_aircraft_capacity = max_cap;
    }

    ship::airvector AircraftCarrier::getAircrafts() {
        return aircrafts; // TODO Вектор самолетов переделать
    }

    void AircraftCarrier::modifyAircrafts(const std::vector<Aircraft> &updated_aircrafts) {
        // TODO модификация информации о самолетах (авианосец)
    }

    void AircraftCarrier::bomberAttack(const ship::coordinate &target_coordinates) {
        // TODO атака бомбардировщика и вылет (авианосец)
    }

    void AircraftCarrier::interceptorAttack(const std::vector<Aircraft> &enemy_aircraft) {
        // TODO Урон противнику от истребителей и вылет (авианосец)
    }

} //namespace acg
