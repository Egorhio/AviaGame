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
        weapon->setActive(true);
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

    // ? МОДЕЛИРОВАНИЕ НАЛЕТА (ОДНОПОТОЧНЫЙ _ МНОГОПОТОЧНЫЙ _ РЕЖИМЫ)


    MissionError Mission::simulateAirRaid(const std::string& carrier_callsign,
                                          const ship::coordinate& target_coordinates) {
        if (carrier_callsign.empty()) return MissionError::EMPTY_CALLSIGN;

        Ship* carrier_ship = shipGroupTable.getShip(carrier_callsign);
        if (!carrier_ship) return MissionError::SHIP_NOT_FOUND;

        // Проверяем тип корабля
        Ship::shiptype ship_type = carrier_ship->getShipType();

        auto* carrier = ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                        dynamic_cast<IAircraftCarrier*>(carrier_ship) : nullptr;
        auto* aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                        dynamic_cast<IAviatorCruiser*>(carrier_ship) : nullptr;

        if (!carrier && !aviator) return MissionError::INVALID_SHIP_TYPE;

        // Получаем список самолетов атакующего корабля
        ship::airvector attacking_aircraft;
        if (carrier) {
            attacking_aircraft = carrier->getAircrafts();
            carrier->interceptorAttack(attacking_aircraft);
            carrier->bomberAttack(target_coordinates); // Добавляем бомбардировку
        } else {
            attacking_aircraft = aviator->getAircrafts();
            aviator->interceptorAttack(attacking_aircraft);
            aviator->bomberAttack(target_coordinates); // Добавляем бомбардировку
        }

        // Ответный огонь от всех кораблей группы
        auto iter = shipGroupTable.getIterator();
        while (iter.hasNext()) {
            auto [_, ship] = iter.get();
            if (ship->getShipType() == Ship::shiptype::CRUISER) {
                auto *defender = dynamic_cast<ICruiser *>(ship);
                if (defender)
                    defender->fireAtAircraft(attacking_aircraft);
            }
            if (ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                auto* defender = dynamic_cast<IAviatorCruiser*>(ship);
                if (defender)
                    defender->fireAtAircraft(attacking_aircraft);
            }
            iter.next();
        }

        return MissionError::SUCCESS;
    }
int x = 0;
    MissionError Mission::MULTIsimulateAirRaid(const std::string& carrier_callsign,
                                               const ship::coordinate& target_coordinates) {
        if (carrier_callsign.empty()) return MissionError::EMPTY_CALLSIGN;
        Ship* carrier_ship = shipGroupTable.getShip(carrier_callsign);
        if (!carrier_ship) return MissionError::SHIP_NOT_FOUND;

        Ship::shiptype ship_type = carrier_ship->getShipType();
        auto* carrier = ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                        dynamic_cast<IAircraftCarrier*>(carrier_ship) : nullptr;
        auto* aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                        dynamic_cast<IAviatorCruiser*>(carrier_ship) : nullptr;
        if (!carrier && !aviator) return MissionError::INVALID_SHIP_TYPE;

        ship::airvector attacking_aircraft;
        std::mutex aircraft_mutex; // Мьютекс для защиты вектора самолётов

        // Получаем список самолетов
        if (carrier) {
            attacking_aircraft = carrier->getAircrafts();
        } else {
            attacking_aircraft = aviator->getAircrafts();
        }

        // Создаем потоки для атаки истребителей и бомбардировщиков
        std::thread interceptor_thread([&]() {
            if (carrier) {
                carrier->interceptorAttack(attacking_aircraft);
            } else {
                aviator->interceptorAttack(attacking_aircraft);
            }
        });

        std::thread bomber_thread([&]() {
            if (carrier) {
                carrier->bomberAttack(target_coordinates);
            } else {
                aviator->bomberAttack(target_coordinates);
            }
        });

        // Создаем вектор потоков для ответного огня
        std::vector<std::thread> defense_threads;
        auto iter = shipGroupTable.getIterator();
        while (iter.hasNext()) {
            auto [_, ship] = iter.get();
            if (ship->getShipType() == Ship::shiptype::CRUISER ||
                ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                defense_threads.emplace_back([&, ship]() {
                    std::lock_guard<std::mutex> lock_guard(aircraft_mutex);
                    if (ship->getShipType() == Ship::shiptype::CRUISER) {
                        auto *defender = dynamic_cast<ICruiser *>(ship);
                        if (defender)
                            defender->fireAtAircraft(attacking_aircraft);
                    }
                    if (ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                        auto *defender = dynamic_cast<IAviatorCruiser *>(ship);
                        if (defender)
                            defender->fireAtAircraft(attacking_aircraft);
                    }
                });
            }
            iter.next();
        }

        // Ожидаем завершения всех потоков
        interceptor_thread.join();
        bomber_thread.join();
        for (auto& thread : defense_threads) {
            thread.join();
        }

        return MissionError::SUCCESS;
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

    std::string Mission::getCaptainRank() const {
        return captain_rank;
    }

    void Mission::setCaptainRank(const std::string& rank) {
        if (rank.empty()) {
            throw std::invalid_argument("Captain rank cannot be empty");
        }
        captain_rank = rank;
    }

    bool Mission::saveState(const std::string& filename) const {
        std::ofstream file(filename);
        // Проверяем, удалось ли открыть файл
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть или создать файл: " << filename << std::endl;
            return false;
        }

        json mission_state;

        // Добавляем новые поля из Mission.h
        mission_state["commander"] = commander;
        mission_state["max_ships"] = max_ships;
        mission_state["budget"] = budget;
        mission_state["spent_sum"] = spent_sum;
        mission_state["saved_units_cost"] = saved_units_cost;
        mission_state["damage_per_group"] = damage_per_group;
        mission_state["necessary_damage"] = necessary_damage;
        mission_state["total_enemy_cost"] = total_enemy_cost;
        mission_state["base_sizes"] = {
                {"baseA", size_baseA},
                {"baseB", size_baseB}
        };
        mission_state["bases"] = {
                {"baseA", {baseA_coordinates.first, baseA_coordinates.second}},
                {"baseB", {baseB_coordinates.first, baseB_coordinates.second}}
        };

        // Сохраняем информацию о кораблях
        mission_state["ships"] = json::array();
        auto iter = shipGroupTable.getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            json ship_data;
            ship_data["call_sign"] = callSign;
            ship_data["type"] = ship->getShipType();
            ship_data["name"] = ship->getName();
            ship_data["captain"] = {
                    {"rank", ship->getCaptainRank()},
                    {"name", ship->getCaptainName()}
            };
            // Добавляем расширенную информацию о корабле
            ship_data["stats"] = {
                    {"speed", ship->getSpeed()},
                    {"durability", ship->getDurability()},
                    {"cost", ship->getCost()},
                    {"total_cost", ship->calculateTotalCost()}
            };

            // Сохраняем координаты
            ship_data["coordinates"] = {
                    {"current", {ship->getCurrentCoordinates().first,
                                        ship->getCurrentCoordinates().second}},
                    {"destination", {ship->getDestinationCoordinates().first,
                                        ship->getDestinationCoordinates().second}}
            };

            Ship::shiptype ship_type = ship->getShipType();

            auto *carrier = ship_type == Ship::shiptype::AIRCRAFTCARRIER ?
                            dynamic_cast<IAircraftCarrier *>(ship) : nullptr;
            auto *aviator = ship_type == Ship::shiptype::AVIATORCRUISER ?
                            dynamic_cast<IAviatorCruiser *>(ship) : nullptr;
            auto *cruiser = ship_type == Ship::shiptype::CRUISER ?
                            dynamic_cast<ICruiser *>(ship) : nullptr;
            // Сохраняем данные для авианосцев и авианесущих крейсеров
            if (carrier) {
                saveAircraftData(ship_data, carrier);
            }
            if (aviator) {
                saveAircraftData(ship_data, aviator);
                saveArmamentData(ship_data, aviator);
            }
            if (cruiser) {
                saveArmamentData(ship_data, cruiser);
            }

            mission_state["ships"].push_back(ship_data);
            iter.next();
        }

        try {
            file << mission_state.dump(4);
            file.flush(); // Принудительная запись буфера
            file.close();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка записи: " << e.what() << std::endl;
            return false;
        }

    }

    bool Mission::loadState(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл: " << filename << std::endl;
            return false;
        }

        try {
            json mission_state;
            file >> mission_state;

            // Загружаем основные параметры миссии
            commander = mission_state["commander"];
            max_ships = mission_state["max_ships"];
            budget = mission_state["budget"];
            spent_sum = mission_state["spent_sum"];
            saved_units_cost = mission_state["saved_units_cost"];
            damage_per_group = mission_state["damage_per_group"];
            necessary_damage = mission_state["necessary_damage"];
            total_enemy_cost = mission_state["total_enemy_cost"];

            // Загружаем размеры баз
            size_baseA = mission_state["base_sizes"]["baseA"];
            size_baseB = mission_state["base_sizes"]["baseB"];

            // Загружаем координаты баз
            auto baseA = mission_state["bases"]["baseA"];
            auto baseB = mission_state["bases"]["baseB"];
            baseA_coordinates = {baseA[0], baseA[1]};
            baseB_coordinates = {baseB[0], baseB[1]};

            // Очищаем текущую таблицу кораблей
            shipGroupTable.clear();

            // Загружаем корабли
            for (const auto& ship_data : mission_state["ships"]) {
                Ship* ship = nullptr;
                auto type = static_cast<Ship::shiptype>(ship_data["type"]);

                // Создаем корабль соответствующего типа
                switch(type) {
                    case Ship::shiptype::AIRCRAFTCARRIER:
                        ship = new AircraftCarrier(type, ship_data["name"],
                                                   ship_data["captain"]["rank"], ship_data["captain"]["name"],
                                                   ship_data["stats"]["speed"], ship_data["stats"]["durability"],
                                                   ship_data["stats"]["cost"]);
                        break;
                    case Ship::shiptype::CRUISER:
                        ship = new Cruiser(type, ship_data["name"],
                                           ship_data["captain"]["rank"], ship_data["captain"]["name"],
                                           ship_data["stats"]["speed"], ship_data["stats"]["durability"],
                                           ship_data["stats"]["cost"], 5, 1000);
                        break;
                    case Ship::shiptype::AVIATORCRUISER:
                        ship = new AviatorCruiser(type, ship_data["name"],
                                                  ship_data["captain"]["rank"], ship_data["captain"]["name"],
                                                  ship_data["stats"]["speed"], ship_data["stats"]["durability"],
                                                  ship_data["stats"]["cost"], 5, 1000, 5);
                        break;
                }

                if (ship) {
                    // Устанавливаем координаты
                    auto current = ship_data["coordinates"]["current"];
                    auto destination = ship_data["coordinates"]["destination"];
                    ship->setCurrentCoordinates({current[0], current[1]});
                    ship->setDestinationCoordinates({destination[0], destination[1]});

                    // Загружаем самолеты и вооружение в зависимости от типа корабля
                    loadAircraftData(ship_data, ship);
                    loadArmamentData(ship_data, ship);

                    // Добавляем корабль в таблицу
                    shipGroupTable.addShip(ship_data["call_sign"], ship);
                }
            }

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
            return false;
        }
    }

    void saveAircraftData(json& ship_data, const auto* carrier) {
        if (!carrier) return;

        ship_data["aircraft"] = json::array();
        auto aircrafts = carrier->getAircrafts();

        for (const auto& [aircraft, coords] : aircrafts) {
            json aircraft_data;
            aircraft_data["type"] = aircraft.getType() == Aircraft::AircraftType::FIGHTER ? "FIGHTER" : "ATTACK";
            aircraft_data["damage"] = aircraft.getDamage();
            aircraft_data["active"] = aircraft.getActive();
            aircraft_data["durability"] = aircraft.getDurability();
            aircraft_data["speed"] = aircraft.getSpeed();
            aircraft_data["fuel_consumption"] = aircraft.getFuelConsumption();
            aircraft_data["fuel_capacity"] = aircraft.getFuelCapacity();
            aircraft_data["refuel_speed"] = aircraft.getRefuelSpeed();
            aircraft_data["cost"] = aircraft.getCost();
            aircraft_data["attack_radius"] = aircraft.getAttackRadius();
            aircraft_data["coordinates"] = {
                    {"x", coords.first},
                    {"y", coords.second}
            };

            ship_data["aircraft"].push_back(aircraft_data);
        }
    }

    void saveArmamentData(json& ship_data, auto* aviator) {
        if (!aviator) return;

        ship_data["armament"] = json::array();
        auto weapons = aviator->getArmament();

        for (const auto& weapon : weapons) {
            json weapon_data;
            weapon_data["name"] = weapon.getName();
            weapon_data["type"] = static_cast<int>(weapon.getType());
            weapon_data["ammo_name"] = weapon.getAmmoName();
            weapon_data["active"] = weapon.getActive();
            weapon_data["damage"] = weapon.getDamage();
            weapon_data["range_of_fire"] = weapon.getRangeOfFire();
            weapon_data["rate_of_fire"] = weapon.getRateOfFire();
            weapon_data["max_ammo_capacity"] = weapon.getMaxAmmoCapacity();
            weapon_data["current_ammo"] = weapon.getCurrentAmmo();
            weapon_data["reload_speed"] = weapon.getReloadSpeed();
            weapon_data["cost"] = weapon.getCost();

            ship_data["armament"].push_back(weapon_data);
        }
    }

    void loadAircraftData(const json& ship_data, Ship* ship) {
        if (!ship_data.contains("aircraft")) return;

        auto* carrier = dynamic_cast<IAircraftCarrier*>(ship);
        auto* aviator = dynamic_cast<IAviatorCruiser*>(ship);
        if (!carrier && !aviator) return;

        ship::airvector aircrafts;
        for (const auto& aircraft_data : ship_data["aircraft"]) {
            Aircraft aircraft(
                    aircraft_data["type"] == "FIGHTER" ?
                    Aircraft::AircraftType::FIGHTER : Aircraft::AircraftType::ATTACK,
                    aircraft_data["damage"],
                    aircraft_data["active"],
                    aircraft_data["durability"],
                    aircraft_data["speed"],
                    aircraft_data["fuel_consumption"],
                    aircraft_data["fuel_capacity"],
                    aircraft_data["refuel_speed"],
                    aircraft_data["cost"],
                    aircraft_data["attack_radius"]
            );
            aircrafts.push_back({aircraft,
                                 {aircraft_data["coordinates"]["x"], aircraft_data["coordinates"]["y"]}});
        }

        if (carrier) carrier->modifyAircrafts(aircrafts);
        else if (aviator) aviator->modifyAircrafts(aircrafts);
    }

    void loadArmamentData(const json& ship_data, Ship* ship) {
        if (!ship_data.contains("armament")) return;

        auto* cruiser = dynamic_cast<ICruiser*>(ship);
        auto* aviator = dynamic_cast<IAviatorCruiser*>(ship);
        if (!cruiser && !aviator) return;

        ship::armvector armament;
        for (const auto& weapon_data : ship_data["armament"]) {
            Armament weapon(
                    weapon_data["name"],
                    static_cast<Armament::ArmamentType>(weapon_data["type"]),
                    weapon_data["damage"],
                    weapon_data["range_of_fire"],
                    weapon_data["rate_of_fire"],
                    weapon_data["max_ammo_capacity"],
                    weapon_data["reload_speed"],
                    weapon_data["cost"]
            );
            weapon.setActive(weapon_data["active"]);
            weapon.setCurrentAmmo(weapon_data["current_ammo"]);
            armament.push_back(weapon);
        }

        if (cruiser) cruiser->modifyArmament(armament);
        else if (aviator) aviator->modifyArmament(armament);
    }


} // namespace acg