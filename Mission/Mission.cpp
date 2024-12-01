#include "Mission.h"

#include <utility>

namespace acg {

    // Проверка бюджета
    bool Mission::checkBudget(double cost) const {
        return (budget - spent_sum >= cost);
    }

    // Получить оставшийся бюджет
    double Mission::getRemainingBudget() const {
        return budget - spent_sum;
    }

// **Купить судно**
    bool Mission::buyShip(const std::string& call_sign, Ship* ship) {
        if (shipGroupTable.getShipCount() >= max_ships || budget < ship->getCost()) {
            return false;
        }
        spent_sum += ship->getCost();
        shipGroupTable.addShip(call_sign, ship);
        return true;
    }

// **Продать судно**
    bool Mission::sellShip(const std::string& call_sign) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (!ship) {
            return false; // Судно не найдено
        }
        spent_sum -= ship->getCost() * 0.8; // Возвращаем 80% стоимости
        shipGroupTable.removeShip(call_sign);
        delete ship;
        return true;
    }

// **Купить самолёт для корабля**
    bool Mission::buyPlaneForShip(const std::string& ship_call_sign, Aircraft* plane) {
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship || getRemainingBudget() < plane->getCost()) {
            return false; // Корабль не найден или не хватает бюджета
        }
        if (ship->getShipType() == Ship::shiptype::CRUISER) {
            return false; // Корабль не может принимать самолёты, это крейсер
        }
        budget -= plane->getCost();
        return true;
    } // TODO Сделать: корабль не может принимать больше самолетов

// **Продать самолёт с корабля**
    bool Mission::sellPlaneFromShip(const std::string& ship_call_sign, const std::string& plane_id) {
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship) {
            return false; // Корабль не найден
        }
        // TODO Убираем самолёт с корабля
//        if (!plane) {
//            return false; // Самолёт не найден
//        }
//        budget += plane->getCost();
//        delete plane; // Удаляем самолёт
        return true;
    }

// **Перевести самолёт на другой корабль**
    bool Mission::transferPlane(const std::string& from_ship_call_sign, const std::string& to_ship_call_sign, const std::string& plane_id) {
        Ship* from_ship = shipGroupTable.getShip(from_ship_call_sign);
        Ship* to_ship = shipGroupTable.getShip(to_ship_call_sign);
        if (!from_ship || !to_ship) {
            return false; // Один из кораблей не найден
        }
//        Aircraft* plane = from_ship->removePlane(plane_id);
//        if (!plane || !to_ship->addPlane(plane)) {
//            if (plane) {
//                from_ship->addPlane(plane); // Возвращаем самолёт обратно, если нельзя передать
//            }
//            return false;
//        } TODO
        return true;
    }

// **Купить оружие для корабля**
    bool Mission::buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon) {
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship || budget < weapon->getCost()) {
            return false;
        }
//        if (!ship->addWeapon(weapon)) {
//            return false; // Корабль не может принять больше оружия
//        } TODO
        budget -= weapon->getCost();
        return true;
    }

// **Продать оружие с корабля**
    bool Mission::sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon) {
        Ship* ship = shipGroupTable.getShip(ship_call_sign);
        if (!ship) {
            return false; // Корабль не найден
        }
        // Armament* weapon = ship->removeWeapon(weapon_id); TODO
        if (!weapon) {
            return false; // Оружие не найдено
        }
        budget += weapon->getCost();
        delete weapon; // Удаляем оружие
        return true;
    }

// **Уничтожить судно**
    void Mission::destroyShip(const std::string& call_sign) {
        Ship* ship = shipGroupTable.getShip(call_sign);
        if (ship) {
            shipGroupTable.removeShip(call_sign);
            delete ship;
        }
    }

// **Учет корабля врага как достигшего цели**
    void Mission::markEnemyAsReached(const std::string& enemy_call_sign) {
        // TODO Заглушка: логика учета, например, может манипулировать состоянием врага
    }

// **Смоделировать налет на группу**
// *Имитация налёта
    void Mission::simulateRaid(const ship::airvector& squad) {
        for (Aircraft plane : squad) {
            // TODO Обработайте логику атаки для каждой группы
            damage_per_group += plane.getDamage(); // Ущерб самолёта
        }
        necessary_damage -= damage_per_group;
    }

    // Количество судов
    int Mission::getShipCount() const {
        return (int) shipGroupTable.getShipCount();
    }



    // * Конструкторы

    Mission::Mission(std::string commander, int maxShips, double budget)
            : commander(std::move(commander)), max_ships(maxShips), budget(budget), spent_sum(0),
              damage_per_group(0), necessary_damage(0), total_enemy_cost(0),
              saved_units_cost(0), size_baseA(0), size_baseB(0) {}

// **Методы доступа: геттеры**

    std::string Mission::getCommander() const {
        return commander;
    }

    int Mission::getMaxShips() const {
        return max_ships;
    }

    double Mission::getBudget() const {
        return budget;
    }

    double Mission::getSpentSum() const {
        return spent_sum;
    }

    double Mission::getDamagePerGroup() const {
        return damage_per_group;
    }

    double Mission::getNecessaryDamage() const {
        return necessary_damage;
    }

    double Mission::getTotalEnemyCost() const {
        return total_enemy_cost;
    }

    double Mission::getSavedUnitsCost() const {
        return saved_units_cost;
    }

    float Mission::getSizeBaseA() const {
        return size_baseA;
    }

    float Mission::getSizeBaseB() const {
        return size_baseB;
    }

    ship::coordinate Mission::getBaseACoordinates() const {
        return baseA_coordinates;
    }

    ship::coordinate Mission::getBaseBCoordinates() const {
        return baseB_coordinates;
    }

// **Методы для модификации: сеттеры**

    void Mission::setCommander(const std::string& name) {
        commander = name;
    }


    void Mission::setMaxShips(int maxShips) {
        max_ships = maxShips;
    }

    void Mission::setBudget(double newBudget) {
        budget = newBudget;
    }

    void Mission::setSpentSum(double spent) {
        spent_sum = spent;
    }

    void Mission::setDamagePerGroup(double damage) {
        damage_per_group = damage;
    }

    void Mission::setNecessaryDamage(double damage) {
        necessary_damage = damage;
    }

    void Mission::setTotalEnemyCost(double cost) {
        total_enemy_cost = cost;
    }

    void Mission::setSavedUnitsCost(double cost) {
        saved_units_cost = cost;
    }

    void Mission::setSizeBaseA(float size) {
        size_baseA = size;
    }

    void Mission::setSizeBaseB(float size) {
        size_baseB = size;
    }

    void Mission::setBaseACoordinates(const ship::coordinate& coordinates) {
        baseA_coordinates = coordinates;
    }

    void Mission::setBaseBCoordinates(const ship::coordinate& coordinates) {
        baseB_coordinates = coordinates;
    }

} // namespace acg