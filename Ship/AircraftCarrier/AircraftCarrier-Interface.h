#ifndef AIRCRAFTCARRIER_INTERFACE_H
#define AIRCRAFTCARRIER_INTERFACE_H

#include "Ship.h"

namespace acg {

    class IAircraftCarrier {
    public:
        virtual ~IAircraftCarrier() = default;

        // **Методы для авианосца**
        virtual void bomberAttack(const ship::coordinate& target_coordinates) = 0; // Вычислить урон от бомбардировщиков и сделать вылет
        virtual void interceptorAttack(const ship::airvector& enemy_aircraft) = 0; // Урон противнику от истребителей и вылет
    };

} //namespace acg

#endif //AIRCRAFTCARRIER_INTERFACE_H
