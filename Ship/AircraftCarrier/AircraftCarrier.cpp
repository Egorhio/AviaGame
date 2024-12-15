#include "AircraftCarrier.h"

namespace acg {

    int AircraftCarrier::getMaxAircraftCapacity() const {
        return max_aircraft_capacity;
    }

    void AircraftCarrier::setMaxAircraftCapacity(int max_cap) {
        if (max_cap < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }
        max_aircraft_capacity = max_cap;
    }

    std::optional<ship::airvector> AircraftCarrier::getAircrafts() {
        return aircrafts;
    }

    void AircraftCarrier::setMaxAircrafts(const ship::airvector& new_aircrafts) {
        aircrafts = new_aircrafts;
    }

    void AircraftCarrier::modifyAircrafts(const ship::airvector& updated_aircrafts) {
        if (updated_aircrafts.size() <= max_aircraft_capacity) {
            aircrafts = updated_aircrafts;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость авиации");
        }
    }

    void AircraftCarrier::bomberAttack(const ship::coordinate& target_coordinates) {
        if (aircrafts.empty()) return;

        auto current_pos = getCurrentCoordinates();
        airothervector available_bombers;

        for (auto& [aircraft, aircraft_pos] : aircrafts) {
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

    double AircraftCarrier::calculateDistance(const ship::coordinate& target_coordinates, const ship::coordinate& current_pos) {
        return std::sqrt(
                std::pow(target_coordinates.first - current_pos.first, 2) +
                std::pow(target_coordinates.second - current_pos.second, 2)
        );
    }

    void AircraftCarrier::executeAttackWaves(airothervector& available_bombers, double distance) {
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


    void AircraftCarrier::interceptorAttack(const ship::airvector& enemy_aircraft) {
        if (aircrafts.empty()) return;
        auto ready_fighters = getReadyFighters(); // Получаем готовых истребителей
        if (ready_fighters.empty()) return;
        assignTargetsToFighters(enemy_aircraft, ready_fighters); // Распределяем цели
    }

// ▎Функция 1: Получение списка готовых истребителей
    airothervector AircraftCarrier::getReadyFighters() {
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
    void AircraftCarrier::assignTargetsToFighters(const ship::airvector& enemy_aircraft,
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
    Aircraft* AircraftCarrier::findBestFighter(airothervector& ready_fighters, double distance) {
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


} //namespace acg
