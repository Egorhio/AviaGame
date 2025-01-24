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

    void AviatorCruiser::bomberAttack(const ship::coordinate& target_coordinates) {
        // Используем статическую переменную для отслеживания количества вызовов
        static int move_call_count = 0;
        const int move_interval_calls = 8; // Интервал обновления движения в количестве вызовов

        // Увеличиваем счетчик вызовов
        move_call_count++;

        // Если количество вызовов меньше интервала, выходим
        if (move_call_count < move_interval_calls) {
            return;
        }

        // Сбрасываем счетчик после достижения интервала
        move_call_count = 0;

        // Существующая логика...
        if (aircrafts.empty()) return;
        auto current_pos = getCurrentCoordinates();
        airothervector available_bombers;
        for (auto &[aircraft, aircraft_pos]: aircrafts) {
            if (aircraft.getType() == Aircraft::AircraftType::ATTACK &&
                aircraft.getActive() &&
                aircraft.getDurability() > 20) {
                available_bombers.push_back(&aircraft);
            }
        }

        if (available_bombers.empty()) return;

        double distance = calculateDistance(target_coordinates, current_pos);
        executeAttackWaves(available_bombers, distance);

    }

    void AviatorCruiser::executeAttackWaves(airothervector& available_bombers, double distance) {
        const int AIRCRAFT_PER_WAVE = 3;
        int waves = static_cast<int>(available_bombers.size()) / AIRCRAFT_PER_WAVE;
        for (int wave = 0; wave < waves; ++wave) {
            double wave_damage_multiplier = 1.0 - (0.1 * wave); // уменьшение эффективности каждой следующей волны
            for (int i = 0; i < AIRCRAFT_PER_WAVE; ++i) {
                Aircraft* bomber = available_bombers[wave * AIRCRAFT_PER_WAVE + i];
                if (distance <= bomber->getAttackRadius()) {
                    bomber->makeAttackRun(distance);
                    int base_damage = bomber->getDamage();

                    double distance_factor = 1.0 - (distance / bomber->getAttackRadius());
                    double durability_factor = bomber->getDurability() / 100.0;
                    double final_multiplier = wave_damage_multiplier * distance_factor * durability_factor;

                    int final_damage = static_cast<int>(base_damage * final_multiplier);

                    if (final_damage > 0) {
                        int wear = static_cast<int>(final_damage * 0.1 * (1 + distance / bomber->getAttackRadius()));
                        bomber->receiveDamage(wear);
                        bomber->setFuelCapacity(bomber->getFuelCapacity() - bomber->getFuelConsumption() * distance * 2); // туда и обратно
                    }
                }
            }
        }
    }


    void AviatorCruiser::interceptorAttack(const ship::airvector& enemy_aircraft) {
        // Используем статическую переменную для отслеживания количества вызовов
        static int move_call_count = 0;
        const int move_interval_calls = 3; // Интервал обновления движения в количестве вызовов

        // Увеличиваем счетчик вызовов
        move_call_count++;

        // Если количество вызовов меньше интервала, выходим
        if (move_call_count < move_interval_calls) {
            return;
        }

        // Сбрасываем счетчик после достижения интервала
        move_call_count = 0;

        // Существующая логика...
        if (aircrafts.empty()) return;
        auto ready_fighters = getReadyFighters();
        if (ready_fighters.empty()) return;
        assignTargetsToFighters(enemy_aircraft, ready_fighters);


    }

// ▎Функция 1: Получение списка готовых истребителей
    airothervector AviatorCruiser::getReadyFighters() {
        std::vector<Aircraft*> ready_fighters;
        // Группируем истребители, которые готовы к атаке
        for (auto& [aircraft, pos] : aircrafts) {
            if (aircraft.getType() == Aircraft::AircraftType::FIGHTER &&
                aircraft.getActive() &&
                aircraft.getDurability() > 30 &&
                aircraft.getFuelCapacity() > aircraft.getFuelConsumption() * 100) {
                ready_fighters.push_back(&aircraft);
            }
        }
        return ready_fighters;
    }

// ▎Функция 2: Распределение целей между истребителями
    void AviatorCruiser::assignTargetsToFighters(const ship::airvector& enemy_aircraft,
                                                  airothervector& ready_fighters) {
        auto current_pos = getCurrentCoordinates();
        for (const auto& [enemy_id, enemy_pos] : enemy_aircraft) {
            double distance = calculateDistance(enemy_pos, current_pos);
            // Находим ближайший подходящий истребитель
            Aircraft* best_fighter = findBestFighter(ready_fighters, distance);
            if (best_fighter) {
                best_fighter->makeAttackRun(distance);

                // Расход топлива и получение урона
                best_fighter->setFuelCapacity(best_fighter->getFuelCapacity() -
                                              best_fighter->getFuelConsumption() * distance * 2);
                best_fighter->receiveDamage(static_cast<int>(10 + distance * 0.1));
                // Исключаем истребитель из доступного списка
                ready_fighters.erase(
                        std::remove(ready_fighters.begin(), ready_fighters.end(), best_fighter),
                        ready_fighters.end()
                );
            }
        }
    }

// ▎Функция 3: Нахождение лучшего истребителя для атаки
    Aircraft* AviatorCruiser::findBestFighter(airothervector& ready_fighters, double distance) {
        Aircraft* best_fighter = nullptr;
        double best_efficiency = 0;

        for (Aircraft* fighter : ready_fighters) {
            if (distance <= fighter->getEffectiveAttackRadius()) {
                double efficiency = (fighter->getDurability() / 100.0) *
                                    (fighter->getFuelCapacity() / fighter->getFuelConsumption()) *
                                    (1.0 - distance / fighter->getEffectiveAttackRadius());

                // Проверяем, является ли текущий истребитель лучшим
                if (efficiency > best_efficiency) {
                    best_efficiency = efficiency;
                    best_fighter = fighter;
                }
            }
        }
        return best_fighter;
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
        // Используем статическую переменную для отслеживания количества вызовов
        static int move_call_count = 0;
        const int move_interval_calls = 4; // Интервал обновления движения в количестве вызовов

        // Увеличиваем счетчик вызовов
        move_call_count++;

        // Если количество вызовов меньше интервала, выходим
        if (move_call_count < move_interval_calls) {
            return;
        }

        // Сбрасываем счетчик после достижения интервала
        move_call_count = 0;

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
            return; // Только легкие орудия могут быть перезаряжены
        }

        static int reload_timer = 0;
        static bool is_reloading = false;

        if (!is_reloading) {
            reload_timer = 0;
            is_reloading = true;
        }

        auto ammo_info = getAmmoInfo(weapon.getAmmoName());
        reload_timer++;

        // Если прошло достаточно времени для перезарядки
        if (reload_timer >= weapon.getReloadSpeed()) {
            int needed_ammo = weapon.getMaxAmmoCapacity() - weapon.getCurrentAmmo();
            if (needed_ammo <= 0) return;

            int available_ammo = std::min(needed_ammo, ammo_info.quantity);
            if (available_ammo > 0) {
                const_cast<Armament&>(weapon).setCurrentAmmo(
                        weapon.getCurrentAmmo() + available_ammo
                );
                ammo_storage[weapon.getAmmoName()].quantity -= available_ammo;
            }
            is_reloading = false;
            reload_timer = 0;
        }
    }

    void AviatorCruiser::fireAtAircraft(const ship::airvector &enemy_aircraft) {
        // Используем статическую переменную для отслеживания количества вызовов
        static int move_call_count = 0;
        const int move_interval_calls = 4; // Интервал обновления движения в количестве вызовов

        // Увеличиваем счетчик вызовов
        move_call_count++;

        // Если количество вызовов меньше интервала, выходим
        if (move_call_count < move_interval_calls) {
            return;
        }

        // Сбрасываем счетчик после достижения интервала
        move_call_count = 0;

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

    void AviatorCruiser::move() {
        Ship::move();
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