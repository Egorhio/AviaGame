#include "Mission.h"

#include <chrono>
#include <iostream>
#include <vector>

using namespace acg;

// Бенчмарк: сравнение однопоточного (simulateAirRaid) и многопоточного
// (MULTIsimulateAirRaid) моделирования налёта на группах авиации разного размера.
int main() {
    const std::vector<int> test_sizes = {50, 200, 1000, 5000, 20000};

    std::cout << "planes | single (us) | multi (us)\n";
    std::cout << "-------+-------------+-----------\n";

    for (int size : test_sizes) {
        auto build = [size](Mission& m) {
            m.setBudget(1e12);
            m.setMaxShips(10);
            auto* carrier = new AircraftCarrier(
                    Ship::shiptype::AIRCRAFTCARRIER,
                    "CARRIER1", "Captain", "John", 30.0, 100, 500.0);
            carrier->setMaxAircraftCapacity(size + 1);
            m.buyShip("CARRIER1", carrier);
            for (int i = 0; i < size; ++i) {
                Aircraft plane(i % 2 ? Aircraft::AircraftType::ATTACK
                                     : Aircraft::AircraftType::FIGHTER,
                               100, true, 100, 50.0, 10.0, 5000.0, 20.0, 300.0, 500.0);
                m.buyPlaneForShip("CARRIER1", &plane);
            }
            auto* defender = new Cruiser(Ship::shiptype::CRUISER, "DEF1", "Captain", "Jack",
                                         30.0, 100, 500.0, 5, 5000);
            m.buyShip("DEF1", defender);
            Armament aa("AA", Armament::ArmamentType::LIGHT, 40, 5000.0, 30.0, 500, 2.0, 300.0);
            m.buyWeaponForShip("DEF1", &aa);
        };

        Mission single("Commander", 10, 1e12);
        Mission multi("Commander", 10, 1e12);
        build(single);
        build(multi);

        const ship::coordinate target{50.0, 50.0};

        auto t0 = std::chrono::steady_clock::now();
        single.simulateAirRaid("CARRIER1", target);
        auto t1 = std::chrono::steady_clock::now();
        multi.MULTIsimulateAirRaid("CARRIER1", target);
        auto t2 = std::chrono::steady_clock::now();

        std::cout << " " << size << "\t| "
                  << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "\t| "
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "\n";
    }

    return 0;
}
