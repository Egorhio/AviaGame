#ifndef AVIATORCRUISER_H
#define AVIATORCRUISER_H

#include "../AircraftCarrier/AircraftCarrier-Interface.h"
#include "../Cruiser/Cruiser-Interface.h"

namespace acg {
    using airothervector = std::vector<Aircraft*>;

    class AviatorCruiser : public IAircraftCarrier, public ICruiser, public Ship {
    private:
        ship::armvector armament{0};       // Вооружение корабля
        int max_armament_capacity = 0;      // Максимальное количество вооружения
        int storage_capacity = 0;           // Вместимость склада
        ship::ammomap ammo_storage;     // Список боеприпасов на складе (по названию)
        ship::airvector aircrafts{0};      // Вектор самолетов
        int max_aircraft_capacity = 0;      // Максимальное количество самолётов

    public:
        AviatorCruiser() = default;
        ~AviatorCruiser() override = default;

        AviatorCruiser(const AviatorCruiser&) = delete;
        AviatorCruiser& operator=(const AviatorCruiser&) = delete;

        // **Методы для части суда - КРЕЙСЕР**
        [[nodiscard]] std::optional<ship::armvector> getArmament() const; // Получить вооружение
        void modifyArmament(const ship::armvector& new_armament); // Модифицировать вооружение
        [[nodiscard]] std::optional<ship::AmmoInfo> getAmmoInfo(const std::string &ammo_name) const; // Получить информацию о боеприпасах
        void modifyAmmoInfo(const ship::ammomap& ammo_name); // Модифицировать боеприпасы
        [[nodiscard]] int getMaxArmamentCapacity() const;
        void setMaxArmamentCapacity(int arm_capacity);
        [[nodiscard]] int getStorageCapacity() const;
        void setStorageCapacity(int st_c);

        [[nodiscard]] int calculateAvailableAmmoStorage() const override; // Рассчитать доступное место для боеприпасов
        void fireAtShip(const ship::coordinate& target_coordinates) override; // Произвести выстрел по кораблю
        void reloadWeapon(const Armament& weapon) override; // Перезарядить оружие снарядами со склада
        void fireAtAircraft(const ship::airvector& enemy_aircraft) override; // Выстрел по самолётам противникака

        // **Методы для части судна - АВИАНОСЕЦ**
        [[nodiscard]] int getMaxAircraftCapacity() const; // Получить максимальное количество самолетов на борту
        void setMaxAircraftCapacity(int max_cap); // Установить максимальное количество самолетов на борту
        [[nodiscard]] std::optional<ship::airvector> getAircrafts(); // Получить информацию о самолётах
        void modifyAircrafts(const ship::airvector& updated_aircrafts); // Модифицировать информацию о самолётах
        void bomberAttack(const ship::coordinate& target_coordinates) override; // Вычислить урон от бомбардировщиков и сделать вылет
        void interceptorAttack(const ship::airvector& enemy_aircraft) override; // Урон противнику от истребителей и вылет


        void move() override; // Переместить корабль в точку назначения
        [[nodiscard]] double calculateTotalCost() const override;
        void setDestination(const ship::coordinate &new_destination) override;


        // ** Дополнительные методы (аналогичные авианосцу)
        static void executeAttackWaves(airothervector &available_bombers, double distance);
        airothervector getReadyFighters();
        void assignTargetsToFighters(const ship::airvector &enemy_aircraft, airothervector &ready_fighters);
        static Aircraft *findBestFighter(airothervector &ready_fighters, double distance);
    };

} // namespace acg

#endif //AVIATORCRUISER_H