#ifndef MISSION_H
#define MISSION_H

#include "Table.h"

namespace acg {

    class Mission {
    private:
        ShipTable<Ship> shipGroupTable;     // Таблица судов группы
        std::string commander;       // Командующий миссией
        int max_ships;               // Максимальное возможное кол-во кораблей в группе
        double budget;               // Бюджет на миссию
        double spent_sum;            // Потраченная сумма денег
        double damage_per_group;     // Ущерб каждой группы
        double necessary_damage;     // Необходимый ущерб для победы
        double total_enemy_cost;     // Общая стоимость противника
        double saved_units_cost;     // Стоимость спасшихся единиц //TODO Кортеж или раздельное хранение рамера и координат
        float size_baseA;            // Размер базы А
        float size_baseB;            // Размер базы Б
        ship::coordinate baseA_coordinates;  // координаты базы А
        ship::coordinate baseB_coordinates;  // координаты базы Б

    public:
        // Купить/продать судно пользователя
        bool buyShip(const std::string& call_sign, Ship* ship);
        bool sellShip(const std::string& call_sign);

        // Купить/продать самолёт для выбранного корабля
        bool buyPlaneForShip(const std::string& ship_call_sign, Aircraft* plane);
        bool sellPlaneFromShip(const std::string& ship_call_sign, const std::string& plane_id);

        // Перевод самолёта на другое судно
        bool transferPlane(const std::string& from_ship_call_sign, const std::string& to_ship_call_sign, const std::string& plane_id);

        // Купить/продать оружие или боеприпасы для корабля
        bool buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon);
        bool sellWeaponFromShip(const std::string& ship_call_sign, const std::string& weapon_id);

        // Уничтожить судно пользователя/противника
        void destroyShip(const std::string& call_sign);

        // Учет корабля противника как достигшего цели
        void markEnemyAsReached(const std::string& enemy_call_sign);

        // Смоделировать налет группы самолетов на отряд
        void simulateAirRaid(const std::vector<Aircraft*>& squadron);
    };

}

#endif // MISSION_H
