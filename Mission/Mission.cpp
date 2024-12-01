//#include "Mission.h"
//
//namespace acg {
//
//// **Купить судно**
//    bool Mission::buyShip(const std::string& call_sign, Ship* ship) {
//        if (shipGroupTable.getShipCount() >= max_ships || budget < ship->getCost()) {
//            return false; // Невозможно купить — превышено ограничение по числу или средствам
//        }
//        shipGroupTable.addShip(call_sign, ship);
//        budget -= ship->getCost(); // Вычитаем стоимость из бюджета
//        return true;
//    }
//
//// **Продать судно**
//    bool Mission::sellShip(const std::string& call_sign) {
//        Ship* ship = shipGroupTable.getShip(call_sign);
//        if (!ship) {
//            return false; // Судно не найдено
//        }
//        budget += ship->getCost(); // Возмещаем стоимость
//        shipGroupTable.removeShip(call_sign);
//        return true;
//    }
//
//// **Купить самолёт для корабля**
//    bool Mission::buyPlaneForShip(const std::string& ship_call_sign, Aircraft* plane) {
//        Ship* ship = shipGroupTable.getShip(ship_call_sign);
//        if (!ship || budget < plane->getCost()) {
//            return false; // Корабль не найден или не хватает бюджета
//        }
//        if (!ship->addPlane(plane)) {
//            return false; // Корабль не может принять больше самолётов
//        }
//        budget -= plane->getCost();
//        return true;
//    }
//
//// **Продать самолёт с корабля**
//    bool Mission::sellPlaneFromShip(const std::string& ship_call_sign, const std::string& plane_id) {
//        Ship* ship = shipGroupTable.getShip(ship_call_sign);
//        if (!ship) {
//            return false; // Корабль не найден
//        }
//        Plane* plane = ship->removePlane(plane_id); // Убираем самолёт с корабля
//        if (!plane) {
//            return false; // Самолёт не найден
//        }
//        budget += plane->getCost();
//        delete plane; // Удаляем самолёт
//        return true;
//    }
//
//// **Перевести самолёт на другой корабль**
//    bool Mission::transferPlane(const std::string& from_ship_call_sign, const std::string& to_ship_call_sign, const std::string& plane_id) {
//        Ship* from_ship = shipGroupTable.getShip(from_ship_call_sign);
//        Ship* to_ship = shipGroupTable.getShip(to_ship_call_sign);
//        if (!from_ship || !to_ship) {
//            return false; // Один из кораблей не найден
//        }
//        Aircraft* plane = from_ship->removePlane(plane_id);
//        if (!plane || !to_ship->addPlane(plane)) {
//            if (plane) {
//                from_ship->addPlane(plane); // Возвращаем самолёт обратно, если нельзя передать
//            }
//            return false;
//        }
//        return true;
//    }
//
//// **Купить оружие для корабля**
//    bool Mission::buyWeaponForShip(const std::string& ship_call_sign, Weapon* weapon) {
//        Ship* ship = shipGroupTable.getShip(ship_call_sign);
//        if (!ship || budget < weapon->getCost()) {
//            return false;
//        }
//        if (!ship->addWeapon(weapon)) {
//            return false; // Корабль не может принять больше оружия
//        }
//        budget -= weapon->getCost();
//        return true;
//    }
//
//// **Продать оружие с корабля**
//    bool Mission::sellWeaponFromShip(const std::string& ship_call_sign, const std::string& weapon_id) {
//        Ship* ship = shipGroupTable.getShip(ship_call_sign);
//        if (!ship) {
//            return false; // Корабль не найден
//        }
//        Weapon* weapon = ship->removeWeapon(weapon_id);
//        if (!weapon) {
//            return false; // Оружие не найдено
//        }
//        budget += weapon->getCost();
//        delete weapon; // Удаляем оружие
//        return true;
//    }
//
//// **Уничтожить судно**
//    void Mission::destroyShip(const std::string& call_sign) {
//        shipGroupTable.removeShip(call_sign); // Удаляем корабль из таблицы
//    }
//
//// **Учет корабля врага как достигшего цели**
//    void Mission::markEnemyAsReached(const std::string& enemy_call_sign) {
//        // Заглушка: логика учета, например, может манипулировать состоянием врага
//    }
//
//// **Смоделировать налет на группу**
//    void Mission::simulateAirRaid(const std::vector<Plane*>& squadron) {
//        // Заглушка: логика зависит от более глубоких данных об объектах
//        std::cout << "Simulating air raid with " << squadron.size() << " planes." << std::endl;
//    }
//
//} // namespace acg