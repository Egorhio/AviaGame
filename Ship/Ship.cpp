#include "Ship.h"

namespace acg {

    Ship::shiptype Ship::getShipType() const {
        return ship_type;
    }

    void Ship::setShipType(Ship::shiptype type) {
        ship_type = type;
    }

    std::string Ship::getName() const {
        return name;
    }

    void Ship::setName(const std::string& n) {
        name = n;
    }

    std::string Ship::getCaptainRank() const {
        return captain_rank;
    }

    void Ship::setCaptainRank(const std::string& rank) {
        captain_rank = rank;
    }

    std::string Ship::getCaptainName() const {
        return captain_name;
    }

    void Ship::setCaptainName(const std::string& cname) {
        captain_name = cname;
    }

    double Ship::getSpeed() const {
        return speed;
    }

    void Ship::setSpeed(double s) {
        if (speed < 0) {
            throw std::invalid_argument("Speed cannot be negative");
        }
        speed = s; // TODO реализовать похожую механику с выбросом ошибок для всех
    }

    int Ship::getDurability() const {
        return durability;
    }

    void Ship::setDurability(int d) {
        durability = d;
    }

    double Ship::getCost() const {
        return cost;
    }

    void Ship::setCost(double c) {
        cost = c;
    }

    // *Методы работы с координатами

    ship::coordinate Ship::getCurrentCoordinates() const {
        return current_coordinates;
    }

    void Ship::setCurrentCoordinates(const ship::coordinate &coordinates) {
        current_coordinates = coordinates;
    }

    ship::coordinate Ship::getDestinationCoordinates() const {
        return destination_coordinates;
    }

    void Ship::setDestinationCoordinates(const ship::coordinate &coordinates) {
        destination_coordinates = coordinates;
    }

    Ship::Ship(IShip::shiptype type, const std::string &name, const std::string &captain_rank,
               const std::string &captain_name, double speed, int durability, double cost) {
        ship_type = type;
        this->name = name;
        this->captain_rank = captain_rank;
        this->captain_name = captain_name;
        this->speed = speed;
        this->durability = durability;
        this->cost = cost;
    }

    // *Работа с базовыми методами*


    void Ship::move() {
        // Проверяем достижимость точки назначения
        double distance = std::sqrt(
                std::pow(destination_coordinates.first - current_coordinates.first, 2) +
                std::pow(destination_coordinates.second - current_coordinates.second, 2)
        );

        // Рассчитываем время движения
        double time = distance / speed;

        // Постепенное обновление координат
        double dx = (destination_coordinates.first - current_coordinates.first) / time;
        double dy = (destination_coordinates.second - current_coordinates.second) / time;

        current_coordinates.first += dx;
        current_coordinates.second += dy;
    }

    void Ship::receiveDamage(int damage) {
        // Базовое снижение урона в зависимости от типа корабля
        double damage_multiplier = 1.0;
        switch(ship_type) {
            case shiptype::CRUISER:
                damage_multiplier = 0.8; // Крейсеры более устойчивы
                break;
            case shiptype::AIRCRAFTCARRIER:
                damage_multiplier = 1.2; // Авианосцы более уязвимы
                break;
            case shiptype::AVIATORCRUISER:
                damage_multiplier = 1.0; // Стандартный урон
                break;
        }

        int final_damage = static_cast<int>(damage * damage_multiplier);
        durability -= final_damage;

        if (durability < 0) durability = 0;
    }

    double Ship::calculateTotalCost() const {
        double total_cost = cost; // Базовая стоимость корабля

        // Модификаторы стоимости в зависимости от состояния
        double durability_factor = static_cast<double>(durability) / 100.0;
        total_cost *= durability_factor;

//        // Добавляем стоимость вооружения и самолетов (если есть)
//        if (auto armament = getArmament(); !armament.empty()) {
//            for (const auto& weapon : armament) {
//                total_cost += weapon->getCost();
//            }
//        }

        return total_cost;
    }

    void Ship::setDestination(const ship::coordinate& new_destination) {
        // Проверка достижимости точки
        double max_distance = speed * 24.0; // Максимальная дистанция за 24 часа
        double required_distance = std::sqrt(
                std::pow(new_destination.first - current_coordinates.first, 2) +
                std::pow(new_destination.second - current_coordinates.second, 2)
        );

        if (required_distance > max_distance) {
            throw std::invalid_argument("Destination is too far");
        }

        destination_coordinates = new_destination;
    }

} // namespace acg