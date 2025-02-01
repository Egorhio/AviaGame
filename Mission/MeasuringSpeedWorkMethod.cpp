#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#include "Mission.h"

using namespace acg;

int main() {
    // Создаем тестовую миссию
    Mission mission("Commander", 100000, 1000000.0);

// Создаем авианосец
    auto* carrier = new AircraftCarrier(
            Ship::shiptype::AIRCRAFTCARRIER,
            "TestCarrier", "Captain", "John", 30.0, 100, 500.0
    );
    mission.buyShip("CARRIER1", carrier);

    std::vector<size_t> test_sizes = {1000, 5000, 10000, 50000, 100000,  500000};

    for(auto size : test_sizes) {
        // Добавляем самолеты
        for(int i = 0; i < size; i++) {
            auto* aircraft = new Aircraft(
                    Aircraft::AircraftType::FIGHTER,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 1000.0
            );
            mission.buyPlaneForShip("CARRIER1", aircraft);
        }

        // Замеряем время многопоточной версии
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << (int) mission.MULTIsimulateAirRaid("CARRIER1", {1000.0, 1000.0}) << std::endl;
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Size: " << size << " Multi-threaded time: " << duration << "ms\n";
    }


    return 0;
}





#pragma clang diagnostic pop