#include "Cruiser.h"

namespace acg {

    ship::armvector Cruiser::getArmament() const {
        return armament;
    }

    void Cruiser::modifyArmament(const ship::armvector &new_armament) {
        if (new_armament.size() <= max_armament_capacity) {
            armament = new_armament;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость вооружения");
        }
    }

    ship::AmmoInfo Cruiser::getAmmoInfo(const std::string &ammo_name) const {
        auto it = ammo_storage.find(ammo_name);
        if (it != ammo_storage.end()) {
            return it->second;
        }
        return {};
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

    void Cruiser::setStorageCapacity(int st_c) {
        if (st_c < 0) {
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
                auto current_pos = current_coordinates;
                double distance = calculateDistance(target_coordinates, current_pos);
                if (distance <= weapon.getRangeOfFire()) {
                    // Проверяем наличие патронов
                    if (weapon.getCurrentAmmo() <= 0) {
                        // Если патронов нет, пытаемся перезарядить
                        reloadWeapon(weapon);
                    }
                    weapon.shoot();
                }
            }
        }
    }

    void Cruiser::reloadWeapon(const Armament &weapon) {
        static int reload_call_count = 0;
        static bool is_reloading = false;
        const int reload_interval_calls = static_cast<int>(weapon.getReloadSpeed());

        // Проверяем наличие боеприпасов до начала перезарядки
        auto ammo_info = getAmmoInfo(weapon.getAmmoName());
        if (ammo_info.quantity <= 0) {
            return;
        }

        if (!is_reloading) {
            reload_call_count = 0;
            is_reloading = true;
        }

        reload_call_count++;
        // Проверяем, прошло ли достаточно "времени" для перезарядки
        if (reload_call_count >= reload_interval_calls) {
            int needed_ammo = weapon.getMaxAmmoCapacity() - weapon.getCurrentAmmo();
            if (needed_ammo <= 0) {
                return;
            }
            int available_ammo = std::min(needed_ammo, ammo_info.quantity);
            if (available_ammo > 0) {
                const_cast<Armament&>(weapon).setCurrentAmmo(
                        weapon.getCurrentAmmo() + available_ammo
                );
                ammo_storage[weapon.getAmmoName()].quantity -= available_ammo;
            }
            is_reloading = false;
        }
    }

    void Cruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        for (auto &weapon: armament) {
            if (weapon.getActive()) {
                for (const auto &aircraft: enemy_aircraft) {
                    auto current_pos = getCurrentCoordinates();
                    double distance = calculateDistance(aircraft.second, current_pos);
                    if (distance <= weapon.getRangeOfFire()) {
                        weapon.shoot();
                        break;
                    }
                }
            }
        }
    }

    // Рассчитать суммарную стоимость корабля, учитывая стоимость вооружения и боеприпасов
    double Cruiser::calculateTotalCost() const {
        double total_cost = cost; // Базовая стоимость корабля
        // Добавляем стоимость вооружения
        for (const auto& weapon : armament) {
            total_cost += weapon.getCost();
        }
        // Добавляем стоимость боеприпасов
        for (const auto& [ammo_name, ammo_info] : ammo_storage) {
            total_cost += ammo_info.quantity * ammo_info.cost;
        }
        // Учитываем состояние корабля
        double durability_factor = static_cast<double>(getDurability()) / 100.0;
        total_cost *= durability_factor;

        return total_cost;
    }

// Установить новую точку назначения для крейсера
    void Cruiser::setDestination(const ship::coordinate &new_destination) {
        double distance = calculateDistance(new_destination, current_coordinates);
        if (distance > speed * 10) {
            double ratio = (speed * 10) / distance;
            destination_coordinates.first = current_coordinates.first +
                                            (new_destination.first - current_coordinates.first) * ratio;
            destination_coordinates.second = current_coordinates.second +
                                             (new_destination.second - current_coordinates.second) * ratio;
        } else {
            destination_coordinates = new_destination;
        }

    }

    void Cruiser::move() {
        double distance = calculateDistance(destination_coordinates, current_coordinates);
        if (distance > speed) {
            double ratio = speed / distance;
            current_coordinates.first += (destination_coordinates.first - current_coordinates.first) * ratio;
            current_coordinates.second += (destination_coordinates.second - current_coordinates.second) * ratio;
        } else {
            current_coordinates = destination_coordinates;
        }
    }

    Cruiser::Cruiser(Ship::shiptype type, const std::string& name,
                     const std::string& captain_rank, const std::string& captain_name,
                     double speed, int durability, double cost,
                     int max_arm_capacity, int storage_cap)
            : Ship(type, name, captain_rank, captain_name, speed, durability, cost),
              max_armament_capacity(max_arm_capacity),
              storage_capacity(storage_cap)
    {
        if (max_arm_capacity < 0) {
            throw std::invalid_argument("Max armament capacity cannot be negative");
        }
        if (storage_cap < 0) {
            throw std::invalid_argument("Storage capacity cannot be negative");
        }

        armament.clear();
        ammo_storage.clear();
    }

} // namespace acg