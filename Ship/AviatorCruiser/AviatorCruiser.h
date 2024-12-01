#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef AVIATORCRUISER_H
#define AVIATORCRUISER_H

#include "../Aircraft/AircraftCarrier-Interface.h"
#include "../Cruiser/Cruiser-Interface.h"

namespace acg {

    class AviatorCruiser : public IAircraft, public ICruiser, public Ship {
    private:
        ship::armvector armament;       // Вооружение корабля
        int max_armament_capacity;      // Максимальное количество вооружения
        int storage_capacity;           // Вместимость склада
        ship::ammomap ammo_storage;     // Список боеприпасов на складе (по названию)
        ship::airvector aircrafts;      // Вектор самолетов
        int max_aircraft_capacity;      // Максимальное количество самолётов

    public:
        AviatorCruiser() = default;
        ~AviatorCruiser() override = default;

        // **Методы для части суда - КРЕЙСЕР**
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

        // **Методы для части судна - АВИАНОСЕЦ**
        [[nodiscard]] int getMaxAircraftCapacity() const override; // Получить максимальное количество самолетов на борту
        void setMaxAircraftCapacity(int max_cap) override; // Установить максимальное количество самолетов на борту
        [[nodiscard]] ship::airvector getAircrafts() override ; // Получить информацию о самолётах
        void modifyAircrafts(const ship::airvector& updated_aircrafts) override; // Модифицировать информацию о самолётах
        void bomberAttack(const ship::coordinate& target_coordinates) override; // Вычислить урон от бомбардировщиков и сделать вылет
        void interceptorAttack(const ship::airvector& enemy_aircraft) override; // Урон противнику от истребителей и вылет
    };

} // namespace acg

#endif //AVIATORCRUISER_H


#pragma clang diagnostic pop