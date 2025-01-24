#include "AircraftCarrier.h"

namespace acg {

    int AircraftCarrier::getMaxAircraftCapacity() const {
        return max_aircraft_capacity;
    }

    void AircraftCarrier::setMaxAircraftCapacity(int max_cap) {
        if (max_cap < 0) {
            throw std::invalid_argument("Максимальное количество самолётов на борту не может быть отрицательным");
        }
        max_aircraft_capacity = max_cap;
    }

    ship::airvector AircraftCarrier::getAircrafts() const {
        return aircrafts;
    }

    void AircraftCarrier::modifyAircrafts(const ship::airvector& updated_aircrafts) {
        if (updated_aircrafts.size() <= max_aircraft_capacity) {
            aircrafts = updated_aircrafts;
        } else {
            throw std::invalid_argument("Превышена максимальная вместимость авиации");
        }
    }

    void AircraftCarrier::bomberAttack(const ship::coordinate &target_coordinates) {
        if (aircrafts.empty()) return;

        auto current_pos = current_coordinates;
        double distance = calculateDistance(target_coordinates, current_pos);

        const int AIRCRAFT_PER_WAVE = 3;
        airothervector available_bombers;

        // Собираем доступные бомбардировщики
        for (auto &[aircraft, _]: aircrafts) {
            if (aircraft.getType() == Aircraft::AircraftType::ATTACK &&
                aircraft.getActive() &&
                aircraft.getDurability() > 1 &&
                distance <= aircraft.getAttackRadius()) {
                available_bombers.push_back(&aircraft);
            }
        }

        if (available_bombers.empty()) return;

        // Выполняем атаку волнами
        size_t waves = available_bombers.size() / AIRCRAFT_PER_WAVE;
        for (int i = 0; i < waves * AIRCRAFT_PER_WAVE; i++) {
            Aircraft* bomber = available_bombers[i];
            int wave = i / AIRCRAFT_PER_WAVE;

            bomber->makeAttackRun(distance);

            // Расчет итогового урона
            double wave_penalty = 1.0 - (0.1 * wave);
            double distance_factor = 1.0 - (distance / bomber->getAttackRadius());
            double durability_factor = bomber->getDurability() / 100.0;
            int final_damage = static_cast<int>(bomber->getDamage() *
                                                wave_penalty *
                                                distance_factor *
                                                durability_factor);

            // Применение износа и расхода топлива
            if (final_damage > 0) {
                int wear = static_cast<int>(final_damage * 0.1 * (1 + distance / bomber->getAttackRadius()));
                bomber->receiveDamage(wear);
                bomber->setFuelCapacity(bomber->getFuelCapacity() -
                                        bomber->getFuelConsumption() * distance * 2);
            }
        }
    }

    void AircraftCarrier::interceptorAttack(const ship::airvector& enemy_aircraft) {
        if (aircrafts.empty()) return;
        auto current_pos = current_coordinates;
        for (auto& [aircraft, pos] : aircrafts) {
            if (aircraft.getType() == Aircraft::AircraftType::FIGHTER && aircraft.getActive()) {
                for (const auto& [enemy_id, enemy_pos] : enemy_aircraft) {
                    double distance = calculateDistance(enemy_pos, current_pos);
                    aircraft.makeAttackRun(distance);
                    break; // Каждый истребитель атакует только одну цель
                }
            }
        }
    }

    [[nodiscard]] double AircraftCarrier::calculateTotalCost() const {
        double total_cost = getCost(); // Базовая стоимость корабля
        for (const auto& [aircraft, _] : aircrafts) {
            total_cost += aircraft.getCost();
        }
        // Учитываем состояние корабля
        double durability_factor = static_cast<double>(getDurability()) / 100.0;
        total_cost *= durability_factor;

        return total_cost;
    }

    void AircraftCarrier::setDestination(const ship::coordinate& new_destination) {
        double distance = calculateDistance(new_destination, getCurrentCoordinates());
        if (distance > speed * 8) {
            double ratio = (speed * 8) / distance;
            ship::coordinate adjusted_destination = {
                    current_coordinates.first +
                    (new_destination.first - current_coordinates.first) * ratio,
                    current_coordinates.second +
                    (new_destination.second - current_coordinates.second) * ratio
            };
            setDestinationCoordinates(adjusted_destination);
        } else {
            setDestinationCoordinates(new_destination);
        }

    }

    void AircraftCarrier::move() {
        auto destination = getDestinationCoordinates();
        double distance = calculateDistance(destination, getCurrentCoordinates());
        if (distance > speed) {
            double ratio = speed / distance;
            ship::coordinate new_pos = {
                    getCurrentCoordinates().first +
                    (destination.first - getCurrentCoordinates().first) * ratio,
                    getCurrentCoordinates().second +
                    (destination.second - getCurrentCoordinates().second) * ratio
            };
            setCurrentCoordinates(new_pos);
        } else {
            setCurrentCoordinates(destination);
        }
    }

} //namespace acg
