#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef AIRCRAFTCARRIER_H
#define AIRCRAFTCARRIER_H

#include "AircraftCarrier-Interface.h"

namespace acg {

    using airothervector = std::vector<Aircraft*>;

    class AircraftCarrier : public Ship, public IAircraftCarrier {
    private:
        ship::airvector aircrafts;             // Вектор самолетов
        int max_aircraft_capacity;       // Максимальное количество самолётов

    public:
        AircraftCarrier(const AircraftCarrier&) = delete;
        AircraftCarrier& operator=(const AircraftCarrier&) = delete;

        AircraftCarrier() = default;
        ~AircraftCarrier() override = default;

        // **Методы для авианосца**
        [[nodiscard]] int getMaxAircraftCapacity() const; // Получить максимальное количество самолетов на борту
        void setMaxAircraftCapacity(int max_cap); // Установить максимальное количество самолетов на борту
        [[nodiscard]] std::optional<ship::airvector> getAircrafts(); // Получить информацию о самолётах
        void modifyAircrafts(const ship::airvector& updated_aircrafts); // Модифицировать информацию о самолётах
        void bomberAttack(const ship::coordinate& target_coordinates) override; // Вычислить урон от бомбардировщиков и сделать вылет
        void interceptorAttack(const ship::airvector& enemy_aircraft) override; // Урон противнику от истребителей и вылет
        void setMaxAircrafts(const ship::airvector &new_aircrafts);

        // ** Дополнительные методы для дополнения логики работы bomberAttack() и interceptorAttack()

        static double calculateDistance(const ship::coordinate &target_coordinates, const ship::coordinate &current_pos);
        static void executeAttackWaves(airothervector &available_bombers, double distance);

        // ** Дополнительные методы для дополнения логики работы interceptorAttack()

        std::vector<Aircraft *> getReadyFighters();
        void assignTargetsToFighters(const ship::airvector &enemy_aircraft, airothervector &ready_fighters);
        static Aircraft *findBestFighter(airothervector &ready_fighters, double distance);
    };

} // namespace acg

#endif //AIRCRAFTCARRIER_H


#pragma clang diagnostic pop