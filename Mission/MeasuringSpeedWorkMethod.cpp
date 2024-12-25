#include <chrono>
#include "Mission.h"
#include <iostream>

using namespace acg;

void benchmark_raid(Mission& mission, const ship::airvector& squad, int data_size) {
    // Подготовка данных
    for(int i = 0; i < data_size; i++) {
        auto* ship = new AircraftCarrier(/*параметры*/);
        mission.buyShip("SHIP" + std::to_string(i), ship);
    }

    // Замер однопоточной версии
    auto start = std::chrono::high_resolution_clock::now();
    mission.simulateRaid(squad); // однопоточная версия
    auto end = std::chrono::high_resolution_clock::now();
    auto single_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Замер многопоточной версии
    start = std::chrono::high_resolution_clock::now();
    mission.simulateRaidParallel(squad); // многопоточная версия
    end = std::chrono::high_resolution_clock::now();
    auto multi_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Size: " << data_size << "\n";
    std::cout << "Single thread: " << single_thread_time << "ms\n";
    std::cout << "Multi thread: " << multi_thread_time << "ms\n";
}

// Использование:
void test_performance() {
    std::vector<int> sizes = {10, 100, 1000, 10000};
    Mission mission("Test", 100000, 1000000.0);
    ship::airvector squad; // заполнить тестовыми данными

    for(int size : sizes) {
        benchmark_raid(mission, squad, size);
    }
}

void benchmark_raid(Mission& mission, int data_size) {
    // Подготовка тестовых данных
    ship::airvector squad;
    for(int i = 0; i < 10; i++) {  // Фиксированное количество самолетов противника
        Aircraft enemy_aircraft(Aircraft::AircraftType::FIGHTER,
                                100, true, 100, 50.0, 10.0,
                                1000.0, 20.0, 5000.0, 100.0);
        squad.push_back({enemy_aircraft, {static_cast<double>(i * 100),
                                          static_cast<double>(i * 100)}});
    }

    // Добавление кораблей разных типов
    for(int i = 0; i < data_size; i++) {
        if (i % 3 == 0) {
            auto* cruiser = new Cruiser(Ship::shiptype::CRUISER,
                                        "Cruiser" + std::to_string(i),
                                        "Captain", "John", 30.0, 100, 500.0,
                                        5, 1000);
            mission.buyShip("SHIP" + std::to_string(i), cruiser);
        } else if (i % 3 == 1) {
            auto* carrier = new AircraftCarrier(Ship::shiptype::AIRCRAFTCARRIER,
                                                "Carrier" + std::to_string(i),
                                                "Captain", "Jack", 25.0, 100, 800.0);
            mission.buyShip("SHIP" + std::to_string(i), carrier);
        } else {
            auto* aviator = new AviatorCruiser(Ship::shiptype::AVIATORCRUISER,
                                               "Aviator" + std::to_string(i),
                                               "Captain", "Jim", 28.0, 100, 700.0,
                                               4, 800, 10);
            mission.buyShip("SHIP" + std::to_string(i), aviator);
        }
    }

    // Замер времени выполнения
    auto start = std::chrono::high_resolution_clock::now();
    mission.simulateRaid(squad);
    auto end = std::chrono::high_resolution_clock::now();
    auto single_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    mission.simulateRaidParallel(squad);
    end = std::chrono::high_resolution_clock::now();
    auto multi_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Size: " << data_size << "\n";
    std::cout << "Single thread: " << single_thread_time << "ms\n";
    std::cout << "Multi thread: " << multi_thread_time << "ms\n\n";
}

void benchmarkraid(Mission& mission, int data_size) {
    // Создаем тестовый отряд противника
    ship::airvector squad;
    for(int i = 0; i < 10; i++) {
        Aircraft enemy_aircraft(Aircraft::AircraftType::FIGHTER,
                                100, true, 100, 50.0, 10.0,
                                1000.0, 20.0, 5000.0, 100.0);
        squad.push_back({enemy_aircraft, {static_cast<double>(i * 100),
                                          static_cast<double>(i * 100)}});
    }

    // Добавляем корабли разных типов
    for(int i = 0; i < data_size; i++) {
        if (i % 3 == 0) {
            auto* cruiser = new Cruiser(/*параметры*/);
            mission.buyShip("SHIP" + std::to_string(i), cruiser);
        } else if (i % 3 == 1) {
            auto* carrier = new AircraftCarrier(/*параметры*/);
            mission.buyShip("SHIP" + std::to_string(i), carrier);
        } else {
            auto* aviator = new AviatorCruiser(/*параметры*/);
            mission.buyShip("SHIP" + std::to_string(i), aviator);
        }
    }

    // Замеры времени
    auto start = std::chrono::high_resolution_clock::now();
    mission.simulateRaid(squad);
    auto end = std::chrono::high_resolution_clock::now();
    auto single_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    mission.simulateRaidParallel(squad);
    end = std::chrono::high_resolution_clock::now();
    auto multi_thread_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Size: " << data_size << "\n";
    std::cout << "Single thread: " << single_thread_time << "ms\n";
    std::cout << "Multi thread: " << multi_thread_time << "ms\n\n";
}

//int main() {
//    std::vector<int> sizes = {10, 100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
//    Mission mission("Test Mission", 100000, 1000000.0); // Большой лимит кораблей и бюджета
//
//    // Создаем тестовый отряд вражеских самолетов
//    ship::airvector squad;
//    for(int i = 0; i < 10; i++) {
//        Aircraft enemy_aircraft(Aircraft::AircraftType::FIGHTER,
//                                100, true, 100, 50.0, 10.0,
//                                1000.0, 20.0, 5000.0, 100.0);
//        squad.push_back({enemy_aircraft, {static_cast<double>(i * 100),
//                                          static_cast<double>(i * 100)}});
//    }
//
//    // Запускаем тесты для разных размеров флота
//    for(int size : sizes) {
//        std::cout << "\nTesting fleet size: " << size << std::endl;
//        benchmark_raid(mission, squad, size);
//    }
//
//    return 0;
//}
int main() {
    std::vector<int> sizes = {10, 100, 1000, 10000, 20000, 30000, 40000, 50000};
    Mission mission("Test", 100000, 1000000.0);

    for (int size: sizes) {
        benchmark_raid(mission, size);
    }

    return 0;
}

