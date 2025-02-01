#include "ai.h"

namespace acg {

    void AI::analyzeGameState(Mission* mission) {
        if (!mission) return;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> coord_dist(1, 18);

        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                ship::coordinate new_dest = {
                        static_cast<double>(coord_dist(gen)),
                        static_cast<double>(coord_dist(gen))
                };
                ship->setDestinationCoordinates(new_dest);
            }
            iter.next();
        }
    }

    void AI::planStrategy(Mission* mission) {
        if (!mission) return;

        // Стратегия атаки для авианосцев
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                if (ship->getShipType() == Ship::shiptype::AIRCRAFTCARRIER ||
                    ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {

                    // Поиск целей для атаки
                    auto enemyIter = mission->getShipGroupTable().getIterator();
                    while (enemyIter.hasNext()) {
                        auto [enemyCallSign, enemyShip] = enemyIter.get();
                        if (enemyCallSign.find(aiTeamCallsign) == std::string::npos) {
                            auto enemyPos = enemyShip->getCurrentCoordinates();
                            // Планирование воздушного налета
                            mission->simulateAirRaid(callSign, enemyPos);
                            break;
                        }
                        enemyIter.next();
                    }
                }
            }
            iter.next();
        }
    }

    void AI::executeActions(Mission* mission) {
        if (!mission) return;

        // Выполнение запланированных действий
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                // Движение к цели
                auto dest = ship->getDestinationCoordinates();
                auto current = ship->getCurrentCoordinates();
                if (dest != current) {
                    ship->move();
                }

                // Атака ближайших целей для крейсеров
                if (ship->getShipType() == Ship::shiptype::CRUISER ||
                    ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                    auto* cruiser = dynamic_cast<ICruiser*>(ship);
                    if (cruiser) {
                        auto enemyIter = mission->getShipGroupTable().getIterator();
                        while (enemyIter.hasNext()) {
                            auto [_, enemyShip] = enemyIter.get();
                            cruiser->fireAtShip(enemyShip->getCurrentCoordinates());
                            break;
                        }
                        enemyIter.next();
                    }
                }
            }
            iter.next();
        }
    }

    MissionError AI::generateRandomAIFleet(Mission* mission, const std::string& aiTeamPrefix) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Генерация случайного количества кораблей (например, 3-7)
        std::uniform_int_distribution<> ship_count_dist(3, 7);
        int shipCount = ship_count_dist(gen);

        // Распределения для характеристик
        std::uniform_real_distribution<> speed_dist(20.0, 40.0);
        std::uniform_int_distribution<> durability_dist(50, 100);
        std::uniform_real_distribution<> cost_dist(500.0, 2000.0);
        std::uniform_int_distribution<> type_dist(0, 2);

        for (int i = 0; i < shipCount; i++) {
            // Создаем случайный корабль
            Ship* ship;
            auto type = static_cast<Ship::shiptype>(type_dist(gen));
            std::string callSign = aiTeamPrefix + std::to_string(i);
            // Генерация случайных координат (исключая 0,0 и 19,19)
            std::uniform_int_distribution<> coord_dist(1, 18); // От 1 до 18
            ship::coordinate random_coords;
            bool valid_coords = false;

            while (!valid_coords) {
                random_coords.first = coord_dist(gen);
                random_coords.second = coord_dist(gen);

                // Проверяем, что координаты не совпадают с базами
                if ((random_coords.first != 0 || random_coords.second != 0) &&
                    (random_coords.first != 19 || random_coords.second != 19)) {
                    valid_coords = true;
                }
            }

            switch(type) {
                case Ship::shiptype::AIRCRAFTCARRIER: {
                    auto* carrier = new AircraftCarrier(type, "AI_Carrier", "Captain", "AI",
                                                        speed_dist(gen), durability_dist(gen), cost_dist(gen));
                    carrier->setMaxAircraftCapacity(5);
                    ship = carrier;
                    ship->setCurrentCoordinates(random_coords);
                    mission->buyShip(callSign, ship);
                    // Добавляем случайные самолеты
                    generateRandomAircraft(mission, callSign);
                    break;
                }
                case Ship::shiptype::CRUISER: {
                    auto* cruiser = new Cruiser(type, "AI_Cruiser", "Captain", "AI",
                                                speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000);
                    cruiser->setMaxArmamentCapacity(10);
                    ship = cruiser;
                    ship->setCurrentCoordinates(random_coords);
                    mission->buyShip(callSign, ship);
                    // Добавляем случайное оружие
                    generateRandomWeapons(mission, callSign);
                    break;
                }
                case Ship::shiptype::AVIATORCRUISER: {
                    auto* aviator = new AviatorCruiser(type, "AI_Aviator", "Captain", "AI",
                                                       speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000, 5);
                    aviator->setMaxAircraftCapacity(5);
                    aviator->setMaxArmamentCapacity(10);
                    ship = aviator;
                    ship->setCurrentCoordinates(random_coords);
                    mission->buyShip(callSign, ship);
                    // Добавляем и оружие, и самолеты
                    generateRandomWeapons(mission, callSign);
                    generateRandomAircraft(mission, callSign);
                    break;
                }
            }
        }

        return MissionError::SUCCESS;
    }

    void AI::generateRandomAircraft(Mission* mission, const std::string& callSign) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> count_dist(1, 3);
        std::uniform_int_distribution<> type_dist(0, 1);

        int planeCount = count_dist(gen);
        for(int i = 0; i < planeCount; i++) {
            auto aircraft = Aircraft(
                    type_dist(gen) == 0 ? Aircraft::AircraftType::FIGHTER : Aircraft::AircraftType::ATTACK,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 1000.0
            );
            mission->buyPlaneForShip(callSign, &aircraft);
        }
    }

    void AI::generateRandomWeapons(Mission* mission, const std::string& callSign) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> count_dist(1, 3);
        std::uniform_int_distribution<> type_dist(0, 1);

        int weaponCount = count_dist(gen);
        for(int i = 0; i < weaponCount; i++) {
            auto weapon = Armament(
                    "AI_Weapon_" + std::to_string(i),
                    type_dist(gen) == 0 ? Armament::ArmamentType::LIGHT : Armament::ArmamentType::HEAVY,
                    100, 1000.0, 60.0, 100, 2.0, 200.0
            );
            mission->buyWeaponForShip(callSign, &weapon);
        }
    }
}