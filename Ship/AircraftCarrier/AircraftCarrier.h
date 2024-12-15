#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef AIRCRAFTCARRIER_H
#define AIRCRAFTCARRIER_H

#include "AircraftCarrier-Interface.h"

namespace acg {

    class AircraftCarrier : public Ship, public IAircraftCarrier {
    private:
        ship::airvector aircrafts;             // Вектор самолетов
        int max_aircraft_capacity;       // Максимальное количество самолётов

    public:
        AircraftCarrier() = default;
        ~AircraftCarrier() override = default;

        // **Методы для авианосца**
        [[nodiscard]] int getMaxAircraftCapacity() const override; // Получить максимальное количество самолетов на борту
        void setMaxAircraftCapacity(int max_cap) override; // Установить максимальное количество самолетов на борту
        [[nodiscard]] ship::airvector getAircrafts() override ; // Получить информацию о самолётах
        void modifyAircrafts(const ship::airvector& updated_aircrafts) override; // Модифицировать информацию о самолётах
        void bomberAttack(const ship::coordinate& target_coordinates) override; // Вычислить урон от бомбардировщиков и сделать вылет
        void interceptorAttack(const ship::airvector& enemy_aircraft) override; // Урон противнику от истребителей и вылет
    };

} // namespace acg

#endif //AIRCRAFTCARRIER_H


#pragma clang diagnostic pop