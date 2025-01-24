#include "Mission.h"

namespace acg {

    // Проверка бюджета
    bool Mission::checkBudget(double cost) const {
        return (budget - spent_sum >= cost);
    }

    // Получить оставшийся бюджет
    double Mission::getRemainingBudget() const {
        return budget - spent_sum;
    }

    MissionError Mission::buyShip(const std::string& call_sign, Ship* ship) {
        if (call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        if (shipGroupTable.getShipCount() >= max_ships) return MissionError::MAX_SHIPS_REACHED;
        if (!checkBudget(ship->calculateTotalCost())) return MissionError::INSUFFICIENT_FUNDS;
        shipGroupTable.addShip(call_sign, ship);
        spent_sum += ship->calculateTotalCost();
        return MissionError::SUCCESS;
    }

    MissionError Mission::sellShip(const std::string& call_sign) {
        if (call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        spent_sum -= ship->calculateTotalCost() * 0.7;
        shipGroupTable.removeShip(call_sign);
        return MissionError::SUCCESS;
    }

    MissionError Mission::buyPlaneForShip(const std::string& call_sign, Aircraft* plane) {
        if (call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!plane) return MissionError::AIRCRAFT_NOT_FOUND;
        Ship *ship = shipGroupTable.getShip(call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        if (!checkBudget(plane->getCost())) return MissionError::INSUFFICIENT_FUNDS;
        Ship::shiptype ship_type = ship->getShipType();
        if (ship_type != Ship::shiptype::AIRCRAFTCARRIER && ship_type != Ship::shiptype::AVIATORCRUISER)
            return MissionError::INVALID_SHIP_TYPE;
        auto* aircarrier = ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                dynamic_cast<IAircraftCarrier*>(ship) : nullptr;
        auto* aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                dynamic_cast<IAviatorCruiser*>(ship) : nullptr;
        ship::airvector current_aircraft;
        getAircraftList(aircarrier, aviator, current_aircraft);
        // Добавление самолёта и обновление списка
        current_aircraft.push_back({*plane, {0, 0}});
        try {
            if (aircarrier) {
                aircarrier->modifyAircrafts(current_aircraft);
            } else if (aviator) {
                aviator->modifyAircrafts(current_aircraft);
            }
        } catch (const std::invalid_argument&) {
            return MissionError::STORAGE_FULL;
        }
        // Обновление бюджета
        spent_sum += plane->getCost();
        return MissionError::SUCCESS;
    }

    MissionError Mission::sellPlaneFromShip(const std::string& call_sign, Aircraft* plane) {
        if (call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!plane) return MissionError::AIRCRAFT_NOT_FOUND;
        Ship *ship = shipGroupTable.getShip(call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        Ship::shiptype ship_type = ship->getShipType();
        if (ship_type != Ship::shiptype::AIRCRAFTCARRIER && ship_type != Ship::shiptype::AVIATORCRUISER)
            return MissionError::INVALID_SHIP_TYPE;
        auto *aircarrier = ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                           dynamic_cast<IAircraftCarrier *>(ship) : nullptr;
        auto *aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                        dynamic_cast<IAviatorCruiser *>(ship) : nullptr;
        ship::airvector current_aircraft;
        getAircraftList(aircarrier, aviator, current_aircraft);
        // Поиск и удаление самолета
        for (auto it = current_aircraft.begin(); it != current_aircraft.end(); ++it) {
            if (it->first == *plane) {
                spent_sum -= plane->getCost() * 0.7; // Возврат 70% стоимости
                current_aircraft.erase(it);
                try {
                    if (aircarrier) {
                        aircarrier->modifyAircrafts(current_aircraft);
                    } else if (aviator){
                        aviator->modifyAircrafts(current_aircraft);
                    }
                    return MissionError::SUCCESS;
                } catch (const std::invalid_argument &) {
                    return MissionError::STORAGE_FULL;
                }
            }
        }
        return MissionError::AIRCRAFT_NOT_FOUND;
    }

    // Получение списка самолетов с корабля
    bool Mission::getAircraftList(IAircraftCarrier* aircarrier,
                                  IAviatorCruiser* aviator,
                                  ship::airvector& current_aircraft) {
        if (aircarrier) {
            current_aircraft = aircarrier->getAircrafts();
        } else {
            current_aircraft = aviator->getAircrafts();
        }
        return true;
    }

    bool Mission::getArmamentList(ICruiser* cruiser,
                                  IAviatorCruiser* aviator,
                                  ship::armvector& current_aircraft) {
        if (cruiser) {
            current_aircraft = cruiser->getArmament();
        } else {
            current_aircraft = aviator->getArmament();
        }
        return true;
    }

    MissionError Mission::transferPlane(Aircraft* plane, const std::string& from_ship, const std::string& to_ship) {
        if (from_ship.empty() || to_ship.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!plane) return MissionError::AIRCRAFT_NOT_FOUND;
        // Получаем корабли из таблицы
        Ship* fromShip = shipGroupTable.getShip(from_ship);
        Ship* toShip = shipGroupTable.getShip(to_ship);
        if (!fromShip || !toShip) return MissionError::SHIP_NOT_FOUND;
        // Проверяем типы кораблей
        Ship::shiptype from_ship_type = fromShip->getShipType();
        Ship::shiptype to_ship_type = toShip->getShipType();
        if ((from_ship_type != Ship::shiptype::AIRCRAFTCARRIER &&
                from_ship_type != Ship::shiptype::AVIATORCRUISER) ||
                (to_ship_type != Ship::shiptype::AIRCRAFTCARRIER &&
                to_ship_type != Ship::shiptype::AVIATORCRUISER))
            return MissionError::INVALID_SHIP_TYPE;
        auto* fromCarrier = from_ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                            dynamic_cast<IAircraftCarrier *>(fromShip) : nullptr;
        auto* fromAviator = from_ship_type == Ship::shiptype::AVIATORCRUISER ?
                            dynamic_cast<IAviatorCruiser *>(fromShip) : nullptr;
        auto* toCarrier = to_ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                          dynamic_cast<IAircraftCarrier *>(toShip) : nullptr;
        auto* toAviator = to_ship_type == Ship::shiptype::AVIATORCRUISER ?
                          dynamic_cast<IAviatorCruiser *>(toShip) : nullptr;

        if ((!fromCarrier && !fromAviator) || (!toCarrier && !toAviator))
            return MissionError::INVALID_SHIP_TYPE;
        // Получаем списки самолётов
        ship::airvector fromAircrafts, toAircrafts;
        getAircraftList(fromCarrier, fromAviator, fromAircrafts);
        // Проверяем вместимость принимающего корабля
        int maxCapacity = toCarrier ? toCarrier->getMaxAircraftCapacity() : toAviator->getMaxAircraftCapacity();
        if (toAircrafts.size() >= maxCapacity) return MissionError::STORAGE_FULL;
        // Ищем и перемещаем самолёт
        for (auto it = fromAircrafts.begin(); it != fromAircrafts.end(); ++it) {
            if (it->first == *plane) {
                toAircrafts.push_back(*it);
                fromAircrafts.erase(it);

                // Обновляем списки самолётов на кораблях
                try {
                    if (fromCarrier) fromCarrier->modifyAircrafts(fromAircrafts);
                    else fromAviator->modifyAircrafts(fromAircrafts);
                    if (toCarrier) toCarrier->modifyAircrafts(toAircrafts);
                    else toAviator->modifyAircrafts(toAircrafts);
                    return MissionError::SUCCESS;
                }
                catch (const std::invalid_argument&) {
                    return MissionError::STORAGE_FULL;
                }
            }
        }

        return MissionError::AIRCRAFT_NOT_FOUND;
    }

    MissionError Mission::destroyShip(const std::string& call_sign) {
        if (call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        ship->setDurability(0);
        shipGroupTable.removeShip(call_sign);
        return MissionError::SUCCESS;
    }

    MissionError Mission::buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon) {
        if (ship_call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!weapon) return MissionError::WEAPON_NOT_FOUND;
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        if (!checkBudget(weapon->getCost()))
            return MissionError::INSUFFICIENT_FUNDS;
        Ship::shiptype ship_type = ship->getShipType();
        if (ship_type != Ship::shiptype::CRUISER && ship_type != Ship::shiptype::AVIATORCRUISER)
            return MissionError::INVALID_SHIP_TYPE;
        auto *cruiser = ship_type == Ship::shiptype::CRUISER ?
                        dynamic_cast<ICruiser *>(ship) : nullptr;
        auto *aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                        dynamic_cast<IAviatorCruiser *>(ship) : nullptr;
        if (!cruiser && !aviator) return MissionError::INVALID_SHIP_TYPE;
        ship::armvector current_armament;
        getArmamentList(cruiser, aviator, current_armament);
        current_armament.push_back(*weapon);
        try {
            if (cruiser) {
                cruiser->modifyArmament(current_armament);
            } else {
                aviator->modifyArmament(current_armament);
            }
        } catch (const std::invalid_argument&) {
            return MissionError::STORAGE_FULL;
        }

        spent_sum += weapon->getCost();
        return MissionError::SUCCESS;
    }

    MissionError Mission::sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon) {
        if (ship_call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        if (!weapon) return MissionError::WEAPON_NOT_FOUND;
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;
        Ship::shiptype ship_type = ship->getShipType();
        if (ship_type != Ship::shiptype::CRUISER && ship_type != Ship::shiptype::AVIATORCRUISER)
            return MissionError::INVALID_SHIP_TYPE;
        auto *cruiser = ship_type == Ship::shiptype::CRUISER ?
                           dynamic_cast<ICruiser *>(ship) : nullptr;
        auto *aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                        dynamic_cast<IAviatorCruiser *>(ship) : nullptr;
        ship::armvector current_armament;
        getArmamentList(cruiser, aviator, current_armament);
        // Ищем и удаляем оружие
        for (auto it = current_armament.begin(); it != current_armament.end(); ++it) {
            if (*it == *weapon) {
                spent_sum -= weapon->getCost() * 0.7; // Возврат 70% стоимости
                current_armament.erase(it);
                try {
                    if (cruiser) {
                        cruiser->modifyArmament(current_armament);
                    } else if (aviator) {
                        aviator->modifyArmament(current_armament);
                    }
                    return MissionError::SUCCESS;
                } catch (const std::invalid_argument&) {
                    return MissionError::STORAGE_FULL;
                }
            }
        }
        return MissionError::WEAPON_NOT_FOUND;
    }

    MissionError Mission::markEnemyAsReached(const std::string& enemy_call_sign) {
        if (enemy_call_sign.empty()) return MissionError::EMPTY_CALLSIGN;
        Ship* enemy_ship = shipGroupTable.getShip(enemy_call_sign);
        if (!enemy_ship) return MissionError::SHIP_NOT_FOUND;
        try {
            // Добавляем проверку состояния корабля
            double ship_condition = enemy_ship->getDurability() / 100.0;
            // Рассчитываем стоимость с учётом повреждений
            double base_cost = enemy_ship->calculateTotalCost();
            double adjusted_cost = base_cost * ship_condition;

            // Добавляем бонус за спасение корабля определённого типа
            switch(enemy_ship->getShipType()) {
                case Ship::shiptype::AIRCRAFTCARRIER:
                    adjusted_cost *= 1.5; // Бонус за авианосец
                    break;
                case Ship::shiptype::AVIATORCRUISER:
                    adjusted_cost *= 1.3; // Бонус за авиакрейсер
                    break;
                default:
                    break;
            }

            saved_units_cost += adjusted_cost;
            shipGroupTable.removeShip(enemy_call_sign);
            return MissionError::SUCCESS;
        }
        catch (const std::exception&) {
            return MissionError::SHIP_NOT_FOUND;
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