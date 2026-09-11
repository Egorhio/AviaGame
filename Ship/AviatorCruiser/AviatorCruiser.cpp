#include "AviatorCruiser.h"

namespace acg {

    AviatorCruiser::AviatorCruiser(Ship::shiptype type, const std::string& name,
                                   const std::string& captain_rank, const std::string& captain_name,
                                   double speed, int durability, double cost,
                                   int max_arm_capacity, int storage_cap, int max_aircraft_cap)
            : Ship(type, name, captain_rank, captain_name, speed, durability, cost),
              max_armament_capacity(max_arm_capacity),
              storage_capacity(storage_cap),
              max_aircraft_capacity(max_aircraft_cap)
    {
        if (type != Ship::shiptype::AVIATORCRUISER) {
            throw std::invalid_argument("Invalid ship type for AviatorCruiser");
        }
        if (max_arm_capacity < 0) {
            throw std::invalid_argument("Max armament capacity cannot be negative");
        }
        if (storage_cap < 0) {
            throw std::invalid_argument("Storage capacity cannot be negative");
        }
        if (max_aircraft_cap < 0) {
            throw std::invalid_argument("Max aircraft capacity cannot be negative");
        }

        armament.clear();
        ammo_storage.clear();
        aircrafts.clear();
    }

    int AviatorCruiser::getMaxAircraftCapacity() const {
        return max_aircraft_capacity;
    }

    void AviatorCruiser::setMaxAircraftCapacity(int max_cap) {
        if (max_cap < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }
        max_aircraft_capacity = max_cap;
    }

    ship::airvector AviatorCruiser::getAircrafts() const {
        return aircrafts;
    }

    void AviatorCruiser::modifyAircrafts(const ship::airvector& updated_aircrafts) {
        if (updated_aircrafts.size() <= max_aircraft_capacity) {
            aircrafts = updated_aircrafts;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость авиации");
        }
    }

    double AviatorCruiser::bomberAttack(const ship::coordinate& target_coordinates) {
        if (aircrafts.empty()) return 0.0;

        auto current_pos = current_coordinates;
        double distance = calculateDistance(target_coordinates, current_pos);

        const int AIRCRAFT_PER_WAVE = 2;
        airothervector available_bombers;

        // Собираем доступные бомбардировщики
        for (auto &[aircraft, _]: aircrafts) {
            if (aircraft.getType() == Aircraft::AircraftType::ATTACK &&
                aircraft.getActive() &&
                aircraft.getDurability() > 10 &&
                distance <= aircraft.getAttackRadius()) {
                available_bombers.push_back(&aircraft);
            }
        }

        if (available_bombers.empty()) return 0.0;

        double total_damage = 0.0;
        // Выполняем атаку волнами (последняя волна может быть неполной)
        for (size_t i = 0; i < available_bombers.size(); i++) {
            Aircraft* bomber = available_bombers[i];
            int wave = static_cast<int>(i / AIRCRAFT_PER_WAVE);

            bomber->makeAttackRun(distance); // расходует топливо и наносит износ

            // Расчет итогового урона с учетом волны
            double wave_penalty = 1.0 - (0.1 * wave);
            double distance_factor = 1.0 - (distance / bomber->getAttackRadius());
            double durability_factor = bomber->getDurability() / 100.0;
            int final_damage = static_cast<int>(bomber->getDamage() *
                                                wave_penalty *
                                                distance_factor *
                                                durability_factor);

            // Дополнительный износ от результативного захода
            if (final_damage > 0) {
                total_damage += final_damage;
                int wear = static_cast<int>(final_damage * 0.1 * (1 + distance / bomber->getAttackRadius()));
                bomber->receiveDamage(wear);
            }
        }
        return total_damage;
    }

    void AviatorCruiser::interceptorAttack(const ship::airvector& enemy_aircraft) {
        if (aircrafts.empty()) return;
        auto current_pos = current_coordinates;
        // Перебираем вражеские самолеты
        for (const auto& [enemy_id, enemy_pos] : enemy_aircraft) {
            double distance = calculateDistance(enemy_pos, current_pos);

            // Ищем лучший истребитель для атаки
            Aircraft* best_fighter = nullptr;
            double best_efficiency = 0;

            // Проверяем все доступные истребители
            for (auto& [aircraft, pos] : aircrafts) {
                if (aircraft.getType() == Aircraft::AircraftType::FIGHTER &&
                    aircraft.getActive() &&
                    aircraft.getDurability() > 30 &&
                    aircraft.getFuelCapacity() > aircraft.getFuelConsumption() * 100) {

                    if (distance <= aircraft.getEffectiveAttackRadius()) {
                        double efficiency = (aircraft.getDurability() / 100.0) *
                                            (aircraft.getFuelCapacity() / aircraft.getFuelConsumption()) *
                                            (1.0 - distance / aircraft.getEffectiveAttackRadius());

                        if (efficiency > best_efficiency) {
                            best_efficiency = efficiency;
                            best_fighter = &aircraft;
                        }
                    }
                }
            }

            // Если нашли подходящий истребитель, выполняем атаку
            if (best_fighter) {
                best_fighter->makeAttackRun(distance); // расходует топливо и наносит износ
                best_fighter->receiveDamage(static_cast<int>(10 + distance * 0.1));
            }
        }
    }

    ship::armvector AviatorCruiser::getArmament() const {
        return armament;
    }

    void AviatorCruiser::modifyArmament(const ship::armvector &new_armament) {
        if (new_armament.size() <= max_armament_capacity) {
            armament = new_armament;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость вооружения");
        }
    }

    ship::AmmoInfo AviatorCruiser::getAmmoInfo(const std::string &ammo_name) const {
        auto it = ammo_storage.find(ammo_name);
        if (it != ammo_storage.end()) {
            return it->second;
        }
        return {};
    }

    ship::ammomap AviatorCruiser::getAmmoStorage() const {
        return ammo_storage;
    }

    void AviatorCruiser::modifyAmmoInfo(const ship::ammomap &ammo_name) {
        int total_ammo = 0;
        for (const auto& [name, info] : ammo_name) {
            total_ammo += info.quantity;
        }

        if (total_ammo <= storage_capacity) {
            ammo_storage = ammo_name;
        } else {
            throw std::invalid_argument("Превышена вместимость склада боеприпасов");
        }
    }

    int AviatorCruiser::getMaxArmamentCapacity() const {
        return max_armament_capacity;
    }

    void AviatorCruiser::setMaxArmamentCapacity(int arm_capacity) {
        if (arm_capacity < 0) {
            throw std::invalid_argument("Armament apacity cannot be negative");
        }
        max_armament_capacity = arm_capacity;
    }

    int AviatorCruiser::getStorageCapacity() const {
        return storage_capacity;
    }

    void AviatorCruiser::setStorageCapacity(int st_c) {
        if (st_c < 0) {
            throw std::invalid_argument("Storage capacity cannot be negative");
        }
        storage_capacity = st_c;
    }

    int AviatorCruiser::calculateAvailableAmmoStorage() const {
        int used_storage = 0;
        for (const auto& [name, info] : ammo_storage) {
            for (const Armament& arm : armament) {
                if (arm.getAmmoName() == name && arm.getType() == Armament::ArmamentType::LIGHT) {
                    used_storage += info.quantity;
                    break;
                }
            }
        }
        return storage_capacity - used_storage;
    }

    void AviatorCruiser::fireAtShip(const ship::coordinate &target_coordinates) {
        for (auto& weapon : armament) {
            if (weapon.getActive() && weapon.getType() == Armament::ArmamentType::LIGHT) {
                auto current_pos = getCurrentCoordinates();
                double distance = calculateDistance(target_coordinates, current_pos);
                if (distance <= weapon.getRangeOfFire()) {
                    weapon.shoot();
                }
            }
        }

    }

    void AviatorCruiser::reloadWeapon(const Armament &weapon) {
        if (weapon.getType() == Armament::ArmamentType::HEAVY) {
            return; // Авианесущий крейсер несёт только лёгкое вооружение
        }

        // Перезарядка снарядами со склада, без статического состояния.
        auto &target = const_cast<Armament &>(weapon);

        const int needed_ammo = target.getMaxAmmoCapacity() - target.getCurrentAmmo();
        if (needed_ammo <= 0) {
            return;
        }

        auto it = ammo_storage.find(target.getAmmoName());
        if (it == ammo_storage.end() || it->second.quantity <= 0) {
            return;
        }

        const int available_ammo = std::min(needed_ammo, it->second.quantity);
        target.setCurrentAmmo(target.getCurrentAmmo() + available_ammo);
        it->second.quantity -= available_ammo;
    }

    void AviatorCruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        for (auto &weapon: armament) {
            if (weapon.getActive() && weapon.getType() == Armament::ArmamentType::LIGHT) {
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

    double AviatorCruiser::calculateTotalCost() const {
        double total_cost = cost; // Базовая стоимость корабля
        // Добавляем стоимость самолётов
        for (const auto& [aircraft, _] : aircrafts) {
            total_cost += aircraft.getCost();
        }
        // Добавляем стоимость вооружения
        for (const auto& weapon : armament) {
            total_cost += weapon.getCost();
        }
        // Добавляем стоимость боеприпасов
        for (const auto& [ammo_name, ammo_info] : ammo_storage) {
            total_cost += ammo_info.quantity * ammo_info.cost;
        }
        // Учитываем состояние корабля
        double durability_factor = static_cast<double>(durability) / 100.0;
        total_cost *= durability_factor;

        return total_cost;
    }

    void AviatorCruiser::setDestination(const ship::coordinate &new_destination) {
        double distance = calculateDistance(new_destination, current_coordinates);
        if (distance > speed * 20) {
            double ratio = (speed * 20) / distance;
            destination_coordinates.first = current_coordinates.first +
                                            (new_destination.first - current_coordinates.first) * ratio;
            destination_coordinates.second = current_coordinates.second +
                                             (new_destination.second - current_coordinates.second) * ratio;
        } else {
            destination_coordinates = new_destination;
        }

    }

}