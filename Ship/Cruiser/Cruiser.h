#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef CRUISER_H
#define CRUISER_H

#include "Cruiser-Interface.h"

namespace acg {

    class Cruiser : public Ship, public ICruiser {
    private:
        ship::armvector armament;             // Вооружение корабля
        int max_armament_capacity;      // Максимальное количество вооружения
        int storage_capacity;           // Вместимость склада
        ship::ammomap ammo_storage;           // Список боеприпасов на складе (по названию)

    public:
        Cruiser() = default;
        ~Cruiser() override = default;

        // **Методы для судна прикрытия (Крейсер)**
        [[nodiscard]] ship::armvector getArmament() const override; // Получить вооружение
        void modifyArmament(const ship::armvector& new_armament) override; // Модифицировать вооружение
        [[nodiscard]] std::optional<ship::AmmoInfo> getAmmoInfo(const std::string &ammo_name) const override; // Получить информацию о боеприпасах
        void modifyAmmoInfo(const ship::ammomap& ammo_name) override; // Модифицировать боеприпасы
        [[nodiscard]] int getMaxArmamentCapacity() const override;
        void setMaxArmamentCapacity(int arm_capacity) override;
        [[nodiscard]] int getStorageCapacity() const override;
        void setStorageCapacity(int st_c) override;

        [[nodiscard]] int calculateAvailableAmmoStorage() const override; // Рассчитать доступное место для боеприпасов
        void fireAtShip(const ship::coordinate& target_coordinates) override; // Произвести выстрел по кораблю
        void reloadWeapon(const Armament& weapon) override; // Перезарядить оружие снарядами со склада
        void fireAtAircraft(const ship::airvector& enemy_aircraft) override; // Выстрел по самолётам противникака
    };

} // namespace acg

#endif //CRUISER_H


#pragma clang diagnostic pop