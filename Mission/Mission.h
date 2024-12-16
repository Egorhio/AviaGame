#ifndef MISSION_H
#define MISSION_H

#include "Table.h"
#include "Cruiser/Cruiser.h"
#include "AircraftCarrier/AircraftCarrier.h"
#include "AviatorCruiser/AviatorCruiser.h"
#include <thread>

namespace acg {

    class Mission {
    private:
        ShipTable<Ship> shipGroupTable;     // Таблица судов группы
        std::string commander = "Unnamed";;       // Командующий миссией
        int max_ships = 0;               // Максимальное возможное кол-во кораблей в группе
        double budget = 0.0;               // Бюджет на миссию
        double spent_sum = 0.0;            // Потраченная сумма денег
        double damage_per_group = 0.0;     // Ущерб каждой группы
        double necessary_damage = 0.0;     // Необходимый ущерб для победы
        double total_enemy_cost = 0.0;     // Общая стоимость противника
        double saved_units_cost = 0.0;     // Стоимость спасшихся единиц
        float size_baseA = 0.0;            // Размер базы А
        float size_baseB = 0.0;            // Размер базы Б
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
        bool transferPlane(Aircraft* plane, const std::string& from_ship, const std::string& to_ship);

        // Методы управления ресурсами
        bool buyPlaneForShip(const std::string& call_sign, Aircraft* plane);
        bool sellPlaneFromShip(const std::string& call_sign, Aircraft* plane);
        bool buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon);
        bool sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon);

        // Боевые действия
        void destroyShip(const std::string& call_sign);
        void markEnemyAsReached(const std::string& enemy_call_sign);
        void simulateRaid(const ship::airvector& squad);

        static bool getAircraftList(AircraftCarrier *aircarrier, AviatorCruiser *aviator, ship::airvector &current_aircraft);

        static bool transferAircraftBetweenShips(Aircraft *plane, ship::airvector &fromAircrafts,
                                          ship::airvector &toAircrafts,
                                          AircraftCarrier *fromCarrier,
                                          AviatorCruiser *fromAviator,
                                          AircraftCarrier *toCarrier,
                                          AviatorCruiser *toAviator);

        static bool getArmamentList(Cruiser *cruiser, AviatorCruiser *aviator, ship::armvector &current_aircraft);
    };

}

#endif // MISSION_H