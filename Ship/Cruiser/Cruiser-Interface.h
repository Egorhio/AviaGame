#ifndef CRUISER_INTERFACE_H
#define CRUISER_INTERFACE_H

#include "Ship.h"

namespace acg {

    class ICruiser {
    public:
        virtual ~ICruiser() = default;

        // **Общие методы для судна прикрытия (Крейсер)**
        [[nodiscard]] virtual ship::armvector getArmament() const = 0; // Получить вооружение
        virtual void modifyArmament(const ship::armvector& new_armament) = 0; // Модифицировать вооружение
        [[nodiscard]] virtual std::optional<ship::AmmoInfo> getAmmoInfo(const std::string &ammo_name) const = 0; // Получить информацию о боеприпасах
        virtual void modifyAmmoInfo(const ship::ammomap& ammo_name) = 0; // Модифицировать боеприпасы
        [[nodiscard]] virtual int getMaxArmamentCapacity() const = 0;
        virtual void setMaxArmamentCapacity(int arm_capacity) = 0;
        [[nodiscard]] virtual int getStorageCapacity() const = 0;
        virtual void setStorageCapacity(int st_c) = 0;

        [[nodiscard]] virtual int calculateAvailableAmmoStorage() const = 0; // Рассчитать доступное место для боеприпасов
        virtual void fireAtShip(const ship::coordinate& target_coordinates) = 0; // Произвести выстрел по кораблю
        virtual void reloadWeapon(const Armament& weapon) = 0; // Перезарядить оружие снарядами со склада
        virtual void fireAtAircraft(const ship::airvector& enemy_aircraft) = 0; // Выстрел по самолётам противникака
    };

}// namespace acg

#endif //CRUISER_INTERFACE_H
