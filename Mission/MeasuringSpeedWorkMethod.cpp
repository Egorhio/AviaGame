#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#include "Mission.h"

namespace acg {
    struct PerformanceData {
        size_t data_size;      // Размер данных (количество кораблей/самолетов)
        double single_thread_time; // Время выполнения однопоточной версии (мс)
        double multi_thread_time;  // Время выполнения многопоточной версии (мс)
    };

    void clearCache() {
        std::vector<int> dummy(1024*1024);
        for(auto& i : dummy) i = rand();
    }

    class PerformanceTester {
    public:
        static std::vector<PerformanceData> measureAirRaidPerformance(
                Mission& mission,
                const std::string& carrier_callsign,
                const ship::coordinate& target,
                const std::vector<size_t>& test_sizes
        ) {
            std::vector<PerformanceData> results;

//            for(auto size : test_sizes) {
//                PerformanceData data{size, 0.0, 0.0};
//
//                // Измерение однопоточной версии
//                auto start = std::chrono::high_resolution_clock::now();
//                mission.simulateAirRaid(carrier_callsign, target);
//                auto end = std::chrono::high_resolution_clock::now();
//                data.single_thread_time = std::chrono::duration<double, std::milli>(end - start).count();
//                data.single_thread_time *= 1000;
//
//                // Измерение многопоточной версии
//                start = std::chrono::high_resolution_clock::now();
//                mission.MULTIsimulateAirRaid(carrier_callsign, target);
//                end = std::chrono::high_resolution_clock::now();
//                data.multi_thread_time = std::chrono::duration<double, std::milli>(end - start).count();
//                data.multi_thread_time *= 1000;
//
//                results.push_back(data);
//            }

            // В методе measureAirRaidPerformance
            for(auto size : test_sizes) {
                PerformanceData data{size, 0.0, 0.0};

                // Добавляем больше кораблей и самолетов для теста
                for(int i = 0; i < size; i++) {
                    auto* aircraft = new Aircraft(
                            Aircraft::AircraftType::FIGHTER,
                            100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 1000.0
                    );
                    mission.buyPlaneForShip("CARRIER1", aircraft);
                }

                // Измерение с повторами для более точных результатов
                clearCache();
                const int REPEAT_COUNT = 100;
                for(int i = 0; i < REPEAT_COUNT; i++) {
                    // Измерение однопоточной версии
                    auto start = std::chrono::high_resolution_clock::now();
                    mission.simulateAirRaid(carrier_callsign, target);
                    auto end = std::chrono::high_resolution_clock::now();
                    data.single_thread_time += std::chrono::duration<double, std::milli>(end - start).count();

                    // Измерение многопоточной версии
                    start = std::chrono::high_resolution_clock::now();
                    mission.MULTIsimulateAirRaid(carrier_callsign, target);
                    end = std::chrono::high_resolution_clock::now();
                    data.multi_thread_time += std::chrono::duration<double, std::milli>(end - start).count();
                }

                // Усредняем результаты
                data.single_thread_time /= REPEAT_COUNT;
                data.multi_thread_time /= REPEAT_COUNT;

                results.push_back(data);
            }

            return results;
        }
    };
}

int main() {
    acg::Mission mission("Commander", 100, 1000000.0);
    // Инициализация кораблей и самолетов...

    std::vector<size_t> test_sizes = {1, 2, 4, 6, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000
    , 200000, 500000, 1000000, 2000000, 5000000, 10000000};
    auto results = acg::PerformanceTester::measureAirRaidPerformance(
            mission,
            "CARRIER1",
            {1000.0, 1000.0},
            test_sizes
    );

    // Вывод результатов для построения графика
    for(const auto& data : results) {
//        std::cout << "Size: " << data.data_size
//                  << " Single: " << data.single_thread_time
//                  << "ms Multi: " << data.multi_thread_time << "ms\n";
        std::cout << "" << data.data_size
                  << "  " << data.single_thread_time
                  << "  " << data.multi_thread_time << "\n";
    }
}
#pragma clang diagnostic pop