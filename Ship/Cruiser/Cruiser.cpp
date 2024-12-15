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
        static auto last_fire_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();

        // Минимальный интервал между залпами (например, 2 секунды)
        const auto min_fire_interval = std::chrono::seconds(2);

        if (current_time - last_fire_time < min_fire_interval) {
            return; // Слишком рано для следующего залпа
        }

        for (auto& weapon : armament) {
            if (weapon.getActive()) {
                auto current_pos = getCurrentCoordinates();
                double distance = calculateDistance(target_coordinates, current_pos);
                if (distance <= weapon.getRangeOfFire()) {
                    weapon.shoot();
                }
            }
        }
        last_fire_time = current_time;
    }

    void Cruiser::reloadWeapon(const Armament &weapon) {
        static auto reload_start_time = std::chrono::steady_clock::now();
        static bool is_reloading = false;

        auto current_time = std::chrono::steady_clock::now();
        auto reload_duration = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>
                (current_time - reload_start_time).count());

        if (!is_reloading) {
            reload_start_time = current_time;
            is_reloading = true;
        }

        auto ammo_info = getAmmoInfo(weapon.getAmmoName());
        if (!ammo_info) return;

        // Если прошло достаточно времени для перезарядки
        if (reload_duration >= weapon.getReloadSpeed()) {
            int needed_ammo = weapon.getMaxAmmoCapacity() - weapon.getCurrentAmmo();
            if (needed_ammo <= 0) return;

            int available_ammo = std::min(needed_ammo, ammo_info->quantity);
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
        static auto last_aa_fire_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();
        // Минимальный интервал между залпами ПВО (например, 1 секунда)
        const auto min_aa_fire_interval = std::chrono::seconds(1);
        if (current_time - last_aa_fire_time < min_aa_fire_interval) {
            return;
        }

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
        last_aa_fire_time = current_time;
    }



    // Рассчитать суммарную стоимость корабля, учитывая стоимость вооружения и боеприпасов
    double Cruiser::calculateTotalCost() const {
        double total_cost = getCost(); // Базовая стоимость корабля
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
        static auto last_destination_change = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();

        // Минимальный интервал между сменой курса (например, 5 секунд)
        const auto min_course_change_interval = std::chrono::seconds(5);

        if (current_time - last_destination_change < min_course_change_interval) {
            return; // Слишком рано для смены курса
        }

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

        last_destination_change = current_time;
    }

    void Cruiser::move() {
        static auto last_move_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();

        // Интервал обновления движения (например, 1 секунда)
        const auto move_interval = std::chrono::seconds(4);

        if (current_time - last_move_time < move_interval) {
            return; // Слишком рано для следующего перемещения
        }

        double distance = calculateDistance(destination_coordinates, current_coordinates);
        if (distance > speed) {
            double ratio = speed / distance;
            current_coordinates.first += (destination_coordinates.first - current_coordinates.first) * ratio;
            current_coordinates.second += (destination_coordinates.second - current_coordinates.second) * ratio;
        } else {
            current_coordinates = destination_coordinates;
        }

        last_move_time = current_time;
    }

} // namespace acg