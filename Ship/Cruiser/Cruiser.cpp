#include "Cruiser.h"

namespace acg {


    ship::armvector Cruiser::getArmament() const {
        return armament; // TODO реализовать по-другому получение вооружения (Крейсер)
    }

    void Cruiser::modifyArmament(const ship::armvector &new_armament) {
        // TODO Модификация вооружения (Крейсер)
    }

    std::optional<ship::AmmoInfo> Cruiser::getAmmoInfo(const std::string &ammo_name) const {
        auto it = ammo_storage.find(ammo_name);
        if (it != ammo_storage.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void Cruiser::modifyAmmoInfo(const ship::ammomap &ammo_name) {
        //TODO Модификация информации о вооружении (Крейсер)
    }

    int Cruiser::getMaxArmamentCapacity() const {
        return max_armament_capacity;
    }

    void Cruiser::setMaxArmamentCapacity(int arm_capacity) {
        max_armament_capacity = arm_capacity;
    }

    int Cruiser::getStorageCapacity() const {
        return storage_capacity;
    }

    void Cruiser::setStorageCapacity(int st_c) {
        storage_capacity = st_c;
    }

    int Cruiser::calculateAvailableAmmoStorage() const {
        return 0; // TODO Рассчет доступного места для боеприпасов (Крейсер)
    }

    void Cruiser::fireAtShip(const ship::coordinate &target_coordinates) {
        // TODO Произвести выстрел по кораблю (Крейсер)
    }

    void Cruiser::reloadWeapon(const Armament &weapon) {
        // TODO Перезарядить оружие снарядами со склада (Крейсер)
    }

    void Cruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        // TODO Выстрел по самолётам противникака  (Крейсер)
    }

} // namespace acg