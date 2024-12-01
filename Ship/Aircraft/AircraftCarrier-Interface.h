#ifndef AIRCRAFT_INTERFACE_H
#define AIRCRAFT_INTERFACE_H

#include "Ship.h"

namespace acg {

    class IAircraft {
    public:
        // **Методы для авианосца**
        [[nodiscard]] virtual int getMaxAircraftCapacity() const = 0; // Получить максимальное количество самолетов на борту
        virtual void setMaxAircraftCapacity(int max_cap) = 0; // Установить максимальное количество самолетов на борту
        [[nodiscard]] virtual ship::airvector getAircrafts() = 0; // Получить информацию о самолётах
        virtual void modifyAircrafts(const ship::airvector& updated_aircrafts) = 0; // Модифицировать информацию о самолётах
        virtual void bomberAttack(const ship::coordinate& target_coordinates) = 0; // Вычислить урон от бомбардировщиков и сделать вылет
        virtual void interceptorAttack(const ship::airvector& enemy_aircraft) = 0; // Урон противнику от истребителей и вылет
    };

} //namespace acg

#endif //AIRCRAFT_INTERFACE_H
