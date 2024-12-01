#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef MISSION_H
#define MISSION_H

#include "Table.h"
#include "Ship.h"

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
        double saved_units_cost;     // Стоимость спасшихся единиц
        float size_baseA;            // Размер базы А
        float size_baseB;            // Размер базы Б
        ship::coordinate baseA_coordinates;  // координаты базы А
        ship::coordinate baseB_coordinates;  // координаты базы Б

        [[nodiscard]] bool checkBudget(double cost) const; // Проверка бюджета

    public:
        Mission() = default;
        ~Mission() = default;

        Mission(std::string commander, int maxShips, double budget);

        // Методы доступа: геттеры
        [[nodiscard]] std::string getCommander() const;
        [[nodiscard]] int getMaxShips() const;
        [[nodiscard]] double getBudget() const;
        [[nodiscard]] double getSpentSum() const;
        [[nodiscard]] double getDamagePerGroup() const;
        [[nodiscard]] double getNecessaryDamage() const;
        [[nodiscard]] double getTotalEnemyCost() const;
        [[nodiscard]] double getSavedUnitsCost() const;
        [[nodiscard]] float getSizeBaseA() const;
        [[nodiscard]] float getSizeBaseB() const;
        [[nodiscard]] ship::coordinate getBaseACoordinates() const;
        [[nodiscard]] ship::coordinate getBaseBCoordinates() const;
        [[nodiscard]] double getRemainingBudget() const;
        [[nodiscard]] int getShipCount() const;

        // Методы для модификации: сеттеры
        void setCommander(const std::string& name);
        void setMaxShips(int maxShips);
        void setBudget(double budget);
        void setSpentSum(double spent);
        void setDamagePerGroup(double damage);
        void setNecessaryDamage(double damage);
        void setTotalEnemyCost(double cost);
        void setSavedUnitsCost(double cost);
        void setSizeBaseA(float size);
        void setSizeBaseB(float size);
        void setBaseACoordinates(const ship::coordinate& coordinates);
        void setBaseBCoordinates(const ship::coordinate& coordinates);

        // Методы управления суднами
        bool buyShip(const std::string& call_sign, Ship* ship);
        bool sellShip(const std::string& call_sign);
        bool transferPlane(const std::string& plane_id, const std::string& from_ship, const std::string& to_ship);

        // Методы управления ресурсами
        bool buyPlaneForShip(const std::string& call_sign, Aircraft* plane);
        bool sellPlaneFromShip(const std::string& call_sign, const std::string& plane_id);
        bool buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon);
        bool sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon);

        // Боевые действия
        void destroyShip(const std::string& call_sign);
        void markEnemyAsReached(const std::string& enemy_call_sign);
        void simulateRaid(const ship::airvector& squad);
    };

}

#endif // MISSION_H


#pragma clang diagnostic pop