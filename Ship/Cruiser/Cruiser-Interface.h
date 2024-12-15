#ifndef CRUISER_INTERFACE_H
#define CRUISER_INTERFACE_H

#include "Ship.h"

namespace acg {

    class ICruiser {
    public:
        virtual ~ICruiser() = default;

        [[nodiscard]] virtual int calculateAvailableAmmoStorage() const = 0; // Рассчитать доступное место для боеприпасов
        virtual void fireAtShip(const ship::coordinate& target_coordinates) = 0; // Произвести выстрел по кораблю
        virtual void reloadWeapon(const Armament& weapon) = 0; // Перезарядить оружие снарядами со склада
        virtual void fireAtAircraft(const ship::airvector& enemy_aircraft) = 0; // Выстрел по самолётам противникака
    };

}// namespace acg

#endif //CRUISER_INTERFACE_H
