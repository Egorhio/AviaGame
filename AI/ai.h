#pragma once

#include "Mission.h"
#include <random>

namespace acg {

    class AI {
    private:
        Mission* mission;
        std::string aiTeamCallsign;

    public:
        explicit AI(Mission* mission, std::string  teamCallsign)
        : mission(mission), aiTeamCallsign(std::move(teamCallsign)) {}

        void makeMove() {
            // Основная логика принятия решений
            analyzeGameState();
            planStrategy();
            executeActions();
        }

        void analyzeGameState();

        void planStrategy();

        void executeActions();

        static MissionError generateRandomAIFleet(Mission* mission, const std::string& aiTeamPrefix) {
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

        static void generateRandomAircraft(Mission* mission, const std::string& callSign) {
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

        static void generateRandomWeapons(Mission* mission, const std::string& callSign) {
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
    };
}


