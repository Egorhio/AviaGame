#include "Cruiser.h"

namespace acg {

    std::optional<ship::armvector> Cruiser::getArmament() const {
        return armament;
    }

    void Cruiser::modifyArmament(const ship::armvector &new_armament) {
        if (new_armament.size() <= max_armament_capacity) {
            armament = new_armament;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость вооружения");
        }
    }

    std::optional<ship::AmmoInfo> Cruiser::getAmmoInfo(const std::string &ammo_name) const {
        auto it = ammo_storage.find(ammo_name);
        if (it != ammo_storage.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void Cruiser::modifyAmmoInfo(const ship::ammomap &new_ammo) {
        int total_ammo = 0;
        for (const auto& [name, info] : new_ammo) {
            total_ammo += info.quantity;
        }

        if (total_ammo <= storage_capacity) {
            ammo_storage = new_ammo;
        } else {
            throw std::invalid_argument("Превышена вместимость склада боеприпасов");
        }
    }

    int Cruiser::getMaxArmamentCapacity() const {
        return max_armament_capacity;
    }

    void Cruiser::setMaxArmamentCapacity(int arm_capacity) {
        if (arm_capacity < 0) {
            throw std::invalid_argument("Armament apacity cannot be negative");
        }
        max_armament_capacity = arm_capacity;
    }

    int Cruiser::getStorageCapacity() const {
        return storage_capacity;
    }

    void Cruiser::setStorageCapacity(int st_c)
    {if (st_c < 0) {
            throw std::invalid_argument("Storage capacity cannot be negative");
        }
        storage_capacity = st_c;
    }

    int Cruiser::calculateAvailableAmmoStorage() const {
        int used_storage = 0;
        for (const auto& [name, info] : ammo_storage) {
            used_storage += info.quantity;
        }
        return storage_capacity - used_storage;
    }

    void Cruiser::fireAtShip(const ship::coordinate &target_coordinates) {
        for (auto& weapon : armament) {
            if (weapon.getActive()) {
                // Проверяем дистанцию до цели
                auto current_pos = getCurrentCoordinates();
                double distance = std::sqrt(
                        std::pow(target_coordinates.first - current_pos.first, 2) +
                        std::pow(target_coordinates.second - current_pos.second, 2)
                );

                if (distance <= weapon.getRangeOfFire()) {
                    weapon.shoot();
                }
            }
        }
    }

    void Cruiser::reloadWeapon(const Armament &weapon) {
        auto ammo_info = getAmmoInfo(weapon.getAmmoName());
        if (!ammo_info) return;

        int needed_ammo = weapon.getMaxAmmoCapacity() - weapon.getCurrentAmmo();
        if (needed_ammo <= 0) return;

        int available_ammo = std::min(needed_ammo, ammo_info->quantity);
        if (available_ammo > 0) {
            // Обновляем количество боеприпасов в оружии и на складе
            const_cast<Armament&>(weapon).setCurrentAmmo(
                    weapon.getCurrentAmmo() + available_ammo
            );
            ammo_storage[weapon.getAmmoName()].quantity -= available_ammo;
        }
    }

    void Cruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        for (auto &weapon: armament) {
            if (weapon.getActive() && weapon.getType() != Armament::ArmamentType::LIGHT) {
                for (const auto &aircraft: enemy_aircraft) {
                    auto current_pos = getCurrentCoordinates();
                    // Проверяем находится ли самолет в зоне поражения
                    double distance = std::sqrt(
                            std::pow(aircraft.second.first - current_pos.first, 2) +
                            std::pow(aircraft.second.second - current_pos.second, 2)
                    );

                    if (distance <= weapon.getRangeOfFire()) {
                        weapon.shoot();
                        break;
                    }
                }
            }
        }
    }

} // namespace acg