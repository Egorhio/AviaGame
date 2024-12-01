#include "AviatorCruiser.h"

namespace acg {


    int AviatorCruiser::getMaxAircraftCapacity() const {
        return max_aircraft_capacity;
    }

    void AviatorCruiser::setMaxAircraftCapacity(int max_cap) {
        max_aircraft_capacity = max_cap;
    }

    ship::airvector AviatorCruiser::getAircrafts() {
        return {}; // TODO Вектор самолетов (Авианесущий крейсер)
    }

    void AviatorCruiser::modifyAircrafts(const ship::airvector &updated_aircrafts) {
        // TODO Модификация информации о самолетах (Авианесущий крейсер)
    }

    void AviatorCruiser::bomberAttack(const ship::coordinate &target_coordinates) {
        // TODO Вычислить урон от бомбардировщиков и сделать вылет (Авианесущий крейсер)
    }

    void AviatorCruiser::interceptorAttack(const ship::airvector &enemy_aircraft) {
        // TODO Урон противнику от истребителей и вылет (Авианесущий крейсер)
    }

    ship::armvector AviatorCruiser::getArmament() const {
        // TODO
        return {};
    }

    void AviatorCruiser::modifyArmament(const ship::armvector &new_armament) {
        // TODO
    }

    std::optional<ship::AmmoInfo> AviatorCruiser::getAmmoInfo(const std::string &ammo_name) const {
        return std::nullopt; // TODO
    }

    void AviatorCruiser::modifyAmmoInfo(const ship::ammomap &ammo_name) {
        // TODO
    }

    int AviatorCruiser::calculateAvailableAmmoStorage() const {
        // TODO
        return {};
    }

    void AviatorCruiser::fireAtShip(const ship::coordinate &target_coordinates) {
        // TODO
    }

    void AviatorCruiser::reloadWeapon(const Armament &weapon) {
        // TODO
    }

    void AviatorCruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        // TODO
    }

    int AviatorCruiser::getMaxArmamentCapacity() const {
        return 0;
    }

    void AviatorCruiser::setMaxArmamentCapacity(int arm_capacity) {

    }

    int AviatorCruiser::getStorageCapacity() const {
        return 0;
    }

    void AviatorCruiser::setStorageCapacity(int st_c) {

    }

}