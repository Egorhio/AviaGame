#include "Mission.h"

#include <utility>

namespace acg {

    // Проверка бюджета
    bool Mission::checkBudget(double cost) const {
        return (budget - spent_sum >= cost);
    }

    // Получить оставшийся бюджет
    double Mission::getRemainingBudget() const {
        return budget - spent_sum;
    }

    bool Mission::buyShip(const std::string& call_sign, Ship* ship) {
        if (!checkBudget(ship->calculateTotalCost())) {
            return false;
        }
        shipGroupTable.addShip(call_sign, ship);
        spent_sum += ship->calculateTotalCost();
        return true;
    }

    bool Mission::sellShip(const std::string& call_sign) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship) return false;
        spent_sum -= ship->calculateTotalCost() * 0.7; // Возврат 70% стоимости
        shipGroupTable.removeShip(call_sign);
        return true;
    }

    bool Mission::buyPlaneForShip(const std::string& call_sign, Aircraft* plane) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship || !checkBudget(plane->getCost())) {
            return false;
        }
        // Проверяем тип корабля и приводим к нужному классу
        auto* aircarrier = dynamic_cast<AircraftCarrier*>(ship);
        auto* aviator = dynamic_cast<AviatorCruiser*>(ship);
        // Если корабль не подходит под оба типа, операция невозможна
        if (!aircarrier && !aviator) {
            return false;
        }
        // Определяем контейнер самолетов у соответствующего типа
        ship::airvector current_aircraft;
        if (!getAircraftList(aircarrier, aviator, current_aircraft)) {
            return false;
        }
        // Добавляем новый самолет в список
        current_aircraft.push_back({*plane, {0, 0}});
        // Обновляем список самолетов
        if (aircarrier) {
            aircarrier->modifyAircrafts(current_aircraft);
        } else {
            aviator->modifyAircrafts(current_aircraft);
        }
        // Обновляем потраченную сумму
        spent_sum += plane->getCost();
        // Возвращаем успех операции
        return true;
    }

    bool Mission::sellPlaneFromShip(const std::string& call_sign, Aircraft* plane) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship) {
            return false; // Корабль не найден
        }
        // Проверяем, является ли корабль носителем авиации
        auto* aircarrier = dynamic_cast<AircraftCarrier*>(ship);
        auto* aviator = dynamic_cast<AviatorCruiser*>(ship);
        if (!aircarrier && !aviator) {
            return false; // Корабль не может нести авиацию
        }
        // Получаем текущий список самолетов
        ship::airvector current_aircraft;
        if (!getAircraftList(aircarrier, aviator, current_aircraft)) {
            return false;
        }
        // Ищем и удаляем самолет
        for (auto it = current_aircraft.begin(); it != current_aircraft.end(); ++it) {
            if (&(it->first) == plane) {
                spent_sum -= plane->getCost() * 0.7; // Возврат 70% стоимости
                current_aircraft.erase(it);
                // Обновляем список самолетов на корабле
                if (aircarrier) {
                    aircarrier->modifyAircrafts(current_aircraft);
                } else {
                    aviator->modifyAircrafts(current_aircraft);
                }
                return true;
            }
        }
        return false; // Самолет не найден
    }

    // Получение списка самолетов с корабля
    bool Mission::getAircraftList(AircraftCarrier* aircarrier,
                                  AviatorCruiser* aviator,
                                  ship::airvector& current_aircraft) {
        if (aircarrier) {
            if (!aircarrier->getAircrafts().has_value()) return false;
            current_aircraft = aircarrier->getAircrafts().value();
        } else {
            if (!aviator->getAircrafts().has_value()) return false;
            current_aircraft = aviator->getAircrafts().value();
        }
        return true;
    }

    bool Mission::getArmamentList(Cruiser* cruiser,
                                  AviatorCruiser* aviator,
                                  ship::armvector& current_aircraft) {
        if (cruiser) {
            if (!cruiser->getArmament().has_value()) return false;
            current_aircraft = cruiser->getArmament().value();
        } else {
            if (!aviator->getArmament().has_value()) return false;
            current_aircraft = aviator->getArmament().value();
        }
        return true;
    }



    bool Mission::transferPlane(Aircraft* plane, const std::string& from_ship, const std::string& to_ship) {
        // Получаем корабли из таблицы
        Ship* fromShip = shipGroupTable.getShip(from_ship);
        Ship* toShip = shipGroupTable.getShip(to_ship);
        if (!fromShip || !toShip) return false;

        // Проверяем, что оба корабля могут работать с авиацией
        auto* fromCarrier = dynamic_cast<AircraftCarrier*>(fromShip);
        auto* fromAviator = dynamic_cast<AviatorCruiser*>(fromShip);
        auto* toCarrier = dynamic_cast<AircraftCarrier*>(toShip);
        auto* toAviator = dynamic_cast<AviatorCruiser*>(toShip);
        if ((!fromCarrier && !fromAviator) || (!toCarrier && !toAviator)) return false;

        // Получаем списки самолётов
        std::optional<ship::airvector> fromAircrafts = fromCarrier ? fromCarrier->getAircrafts() : fromAviator->getAircrafts();
        std::optional<ship::airvector> toAircrafts = toCarrier ? toCarrier->getAircrafts() : toAviator->getAircrafts();
        if (!fromAircrafts || !toAircrafts) return false;

        // Проверяем вместимость принимающего корабля
        int maxCapacity = toCarrier ? toCarrier->getMaxAircraftCapacity() : toAviator->getMaxAircraftCapacity();
        if (toAircrafts->size() >= maxCapacity) return false;

        // Удаляем самолёт из первого корабля и добавляем во второй
        ship::airvector newFromAircrafts = *fromAircrafts;
        ship::airvector newToAircrafts = *toAircrafts;

        return transferAircraftBetweenShips(plane, newFromAircrafts, newToAircrafts, fromCarrier, fromAviator, toCarrier, toAviator);
    }

    bool Mission::transferAircraftBetweenShips(Aircraft* plane, ship::airvector& fromAircrafts,
                                               ship::airvector& toAircrafts,
                                               AircraftCarrier* fromCarrier,
                                               AviatorCruiser* fromAviator,
                                               AircraftCarrier* toCarrier,
                                               AviatorCruiser* toAviator) {
        for (auto it = fromAircrafts.begin(); it != fromAircrafts.end(); ++it) {
            if (&(it->first) == plane) {
                toAircrafts.push_back(*it);
                fromAircrafts.erase(it);

                if (fromCarrier) fromCarrier->modifyAircrafts(fromAircrafts);
                else fromAviator->modifyAircrafts(fromAircrafts);

                if (toCarrier) toCarrier->modifyAircrafts(toAircrafts);
                else toAviator->modifyAircrafts(toAircrafts);

                return true;
            }
        }
        return false;
    }

    void Mission::destroyShip(const std::string& call_sign) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (ship) {
            ship->setDurability(0);
            shipGroupTable.removeShip(call_sign);
        }
    }

    bool Mission::buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon) {
        // Получаем корабль по позывному
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship || !checkBudget(weapon->getCost())) {
            return false; // Корабль не найден или недостаточно средств
        }
        // Проверяем тип корабля и приводим к нужному классу
        auto* cruiser = dynamic_cast<Cruiser*>(ship);
        auto* aviator = dynamic_cast<AviatorCruiser*>(ship);
        if (!cruiser && !aviator) {
            return false; // Корабль не может нести вооружение
        }
        // Получаем текущее вооружение
        ship::armvector current_armament;
        if (!getArmamentList(cruiser, aviator, current_armament)) {
            return false;
        }
        // Добавляем новое оружие
        current_armament.push_back(*weapon);
        // Обновляем вооружение на корабле
        if (cruiser) {
            cruiser->modifyArmament(current_armament);
        } else {
            aviator->modifyArmament(current_armament);
        }
        spent_sum += weapon->getCost();
        return true;
    }

    bool Mission::sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon) {
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship) return false;
        auto* cruiser = dynamic_cast<Cruiser*>(ship);
        auto* aviator = dynamic_cast<AviatorCruiser*>(ship);
        if (!cruiser && !aviator) return false;
        ship::armvector current_armament;
        if (!getArmamentList(cruiser, aviator, current_armament)) {
            return false;
        }
        // Ищем и удаляем оружие
        for (auto it = current_armament.begin(); it != current_armament.end(); ++it) {
            if (&(*it) == weapon) {
                spent_sum -= weapon->getCost() * 0.7; // Возврат 70% стоимости
                current_armament.erase(it);
                if (cruiser) {
                    cruiser->modifyArmament(current_armament);
                } else {
                    aviator->modifyArmament(current_armament);
                }
                return true;
            }
        }
        return false;
    }

    void Mission::markEnemyAsReached(const std::string& enemy_call_sign) {
        Ship* enemy_ship = shipGroupTable.getShip(enemy_call_sign);
        if (!enemy_ship) {
            return;
        }
        // Добавляем стоимость спасшегося корабля
        saved_units_cost += enemy_ship->calculateTotalCost();
        // Удаляем корабль из таблицы
        shipGroupTable.removeShip(enemy_call_sign);
    }


    void Mission::simulateRaid(const ship::airvector& squad) {
        // Создаем потоки для каждой группы кораблей
        std::vector<std::thread> threads;

        auto iter = shipGroupTable.getIterator();
        while (iter.hasNext()) {
            auto [call_sign, ship] = iter.get();

            // Создаем поток для обработки каждого корабля
            threads.emplace_back([this, ship, &squad]() {
                // Обработка в зависимости от типа корабля
                if (auto carrier = dynamic_cast<AircraftCarrier*>(ship)) {
                    carrier->interceptorAttack(squad);
                }
                else if (auto aviator = dynamic_cast<AviatorCruiser*>(ship)) {
                    aviator->interceptorAttack(squad);
                    aviator->fireAtAircraft(squad);
                }
                else if (auto cruiser = dynamic_cast<Cruiser*>(ship)) {
                    cruiser->fireAtAircraft(squad);
                }
            });

            iter.next();
        }

        // Ожидаем завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }
    }



    // * Конструкторы

    Mission::Mission(std::string commander, int maxShips, double budget)
            : commander(std::move(commander)), max_ships(maxShips), budget(budget), spent_sum(0),
              damage_per_group(0), necessary_damage(0), total_enemy_cost(0),
              saved_units_cost(0), size_baseA(0), size_baseB(0) {}

// **Методы доступа: геттеры**

    std::string Mission::getCommander() const {
        return commander;
    }

    int Mission::getMaxShips() const {
        return max_ships;
    }

    double Mission::getBudget() const {
        return budget;
    }

    double Mission::getSpentSum() const {
        return spent_sum;
    }

    double Mission::getDamagePerGroup() const {
        return damage_per_group;
    }

    double Mission::getNecessaryDamage() const {
        return necessary_damage;
    }

    double Mission::getTotalEnemyCost() const {
        return total_enemy_cost;
    }

    double Mission::getSavedUnitsCost() const {
        return saved_units_cost;
    }

    float Mission::getSizeBaseA() const {
        return size_baseA;
    }

    float Mission::getSizeBaseB() const {
        return size_baseB;
    }

    ship::coordinate Mission::getBaseACoordinates() const {
        return baseA_coordinates;
    }

    ship::coordinate Mission::getBaseBCoordinates() const {
        return baseB_coordinates;
    }

// **Методы для модификации: сеттеры**

    void Mission::setCommander(const std::string& name) {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        commander = name;
    }


    void Mission::setMaxShips(int maxShips) {
        if (maxShips < 0) {
            throw std::invalid_argument("Budget cannot be negative");
        }
        max_ships = maxShips;
    }

    void Mission::setBudget(double newBudget) {
        if (newBudget < 0) {
            throw std::invalid_argument("Budget cannot be negative");
        }
        budget = newBudget;
    }

    void Mission::setSpentSum(double spent) {
        if (spent < 0) {
            throw std::invalid_argument("Spent Sum cannot be negative");
        }
        spent_sum = spent;
    }

    void Mission::setDamagePerGroup(double damage) {
        if (damage < 0) {
            throw std::invalid_argument("Damage per group cannot be negative");
        }
        damage_per_group = damage;
    }

    void Mission::setNecessaryDamage(double damage) {
        if (damage < 0) {
            throw std::invalid_argument("Necessary damage cannot be negative");
        }
        necessary_damage = damage;
    }

    void Mission::setTotalEnemyCost(double cost) {
        if (cost < 0) {
            throw std::invalid_argument("Total enemy cost cannot be negative");
        }
        total_enemy_cost = cost;
    }

    void Mission::setSavedUnitsCost(double cost) {
        if (cost < 0) {
            throw std::invalid_argument("Units cost cannot be negative");
        }
        saved_units_cost = cost;
    }

    void Mission::setSizeBaseA(float size) {
        if (size < 0) {
            throw std::invalid_argument("Size base A cannot be negative");
        }
        size_baseA = size;
    }

    void Mission::setSizeBaseB(float size) {
        if (size < 0) {
            throw std::invalid_argument("Size base B cannot be negative");
        }
        size_baseB = size;
    }

    void Mission::setBaseACoordinates(const ship::coordinate& coordinates) {
        baseA_coordinates = coordinates;
    }

    void Mission::setBaseBCoordinates(const ship::coordinate& coordinates) {
        baseB_coordinates = coordinates;
    }

} // namespace acg