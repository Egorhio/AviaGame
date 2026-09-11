#include "ai.h"

namespace acg {

    void AI::analyzeGameState(Mission* mission) {
        if (!mission) return;

        // Флот ИИ держит курс на базу B (цель — довести корабли до базы назначения)
        const ship::coordinate baseB = mission->getBaseBCoordinates();

        for (auto iter = mission->getShipGroupTable().getIterator(); iter.hasNext(); iter.next()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                ship->setDestinationCoordinates(baseB);
            }
        }
    }

    void AI::planStrategy(Mission* mission) {
        if (!mission) return;

        // Командующий ИИ за ход планирует один воздушный налёт: выбирает свой
        // авианесущий корабль и ближайшую к нему цель противника.
        std::string raider;
        std::string victim;
        double best_dist = -1.0;

        for (auto iter = mission->getShipGroupTable().getIterator(); iter.hasNext(); iter.next()) {
            auto [callSign, ship] = iter.get();
            const bool isAI = callSign.find(aiTeamCallsign) != std::string::npos;
            if (!isAI) continue;
            if (ship->getShipType() != Ship::shiptype::AIRCRAFTCARRIER &&
                ship->getShipType() != Ship::shiptype::AVIATORCRUISER) continue;

            for (auto e = mission->getShipGroupTable().getIterator(); e.hasNext(); e.next()) {
                auto [enemyCallSign, enemyShip] = e.get();
                if (enemyCallSign.find(aiTeamCallsign) != std::string::npos) continue;
                double d = calculateDistance(enemyShip->getCurrentCoordinates(),
                                             ship->getCurrentCoordinates());
                if (best_dist < 0 || d < best_dist) {
                    best_dist = d;
                    raider = callSign;
                    victim = enemyCallSign;
                }
            }
        }

        if (best_dist >= 0.0) {
            Ship* target = mission->getShipGroupTable().getShip(victim);
            if (target) mission->simulateAirRaid(raider, target->getCurrentCoordinates());
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

                // Атака ближайшей цели противника для крейсеров
                if (ship->getShipType() == Ship::shiptype::CRUISER ||
                    ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                    if (auto* cruiser = dynamic_cast<ICruiser*>(ship)) {
                        ship::coordinate best_target{};
                        double best_dist = -1.0;
                        for (auto enemyIter = mission->getShipGroupTable().getIterator();
                             enemyIter.hasNext(); enemyIter.next()) {
                            auto [enemyCallSign, enemyShip] = enemyIter.get();
                            if (enemyCallSign.find(aiTeamCallsign) != std::string::npos) continue; // свой
                            double d = calculateDistance(enemyShip->getCurrentCoordinates(),
                                                         ship->getCurrentCoordinates());
                            if (best_dist < 0 || d < best_dist) {
                                best_dist = d;
                                best_target = enemyShip->getCurrentCoordinates();
                            }
                        }
                        if (best_dist >= 0.0) {
                            cruiser->fireAtShip(best_target);
                        }
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

        // Флот противника формируется независимо от бюджета и лимита игрока:
        // расширяем лимиты миссии, чтобы покупки ИИ гарантированно прошли.
        mission->setMaxShips(mission->getMaxShips() + shipCount);
        mission->setBudget(mission->getBudget() + shipCount * 6000.0);

        // Распределения для характеристик
        std::uniform_real_distribution<> speed_dist(20.0, 40.0);
        std::uniform_int_distribution<> durability_dist(50, 100);
        std::uniform_real_distribution<> cost_dist(500.0, 2000.0);
        std::uniform_int_distribution<> type_dist(0, 2);

        for (int i = 0; i < shipCount; i++) {
            // Создаем случайный корабль
            Ship* ship = nullptr;
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

            bool needsAircraft = false;
            bool needsWeapons = false;
            switch(type) {
                case Ship::shiptype::AIRCRAFTCARRIER: {
                    auto* carrier = new AircraftCarrier(type, "AI_Carrier", "Captain", "AI",
                                                        speed_dist(gen), durability_dist(gen), cost_dist(gen));
                    carrier->setMaxAircraftCapacity(5);
                    ship = carrier;
                    needsAircraft = true;
                    break;
                }
                case Ship::shiptype::CRUISER: {
                    auto* cruiser = new Cruiser(type, "AI_Cruiser", "Captain", "AI",
                                                speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000);
                    cruiser->setMaxArmamentCapacity(10);
                    ship = cruiser;
                    needsWeapons = true;
                    break;
                }
                case Ship::shiptype::AVIATORCRUISER: {
                    auto* aviator = new AviatorCruiser(type, "AI_Aviator", "Captain", "AI",
                                                       speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000, 5);
                    aviator->setMaxAircraftCapacity(5);
                    aviator->setMaxArmamentCapacity(10);
                    ship = aviator;
                    needsAircraft = true;
                    needsWeapons = true;
                    break;
                }
            }

            if (!ship) continue;
            ship->setCurrentCoordinates(random_coords);
            if (mission->buyShip(callSign, ship) != MissionError::SUCCESS) {
                delete ship;                 // покупка не прошла — не допускаем утечки
                continue;
            }
            // Учитываем стоимость корабля противника
            mission->setTotalEnemyCost(mission->getTotalEnemyCost() + ship->calculateTotalCost());
            if (needsWeapons)  generateRandomWeapons(mission, callSign);
            if (needsAircraft) generateRandomAircraft(mission, callSign);
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