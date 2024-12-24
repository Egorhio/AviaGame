#include "Ship.h"

namespace acg {
    const double max_possible_speed = 100.0; // максимально возможная скорость в узлах

    Ship::shiptype Ship::getShipType() const {
        return ship_type;
    }

    void Ship::setShipType(Ship::shiptype type) {
        if (type != shiptype::CRUISER &&
            type != shiptype::AIRCRAFTCARRIER &&
            type != shiptype::AVIATORCRUISER) {
            throw std::invalid_argument("Invalid ship type");
        }
        ship_type = type;
    }

    std::string Ship::getName() const {
        return name;
    }

    void Ship::setName(const std::string& n) {
        if (name.empty()) {
            throw std::invalid_argument("Captain name cannot be empty");
        }
        name = n;
    }

    std::string Ship::getCaptainRank() const {
        return captain_rank;
    }

    void Ship::setCaptainRank(const std::string& rank) {
        if (rank.empty()) {
            throw std::invalid_argument("Captain rank cannot be empty");
        }
        captain_rank = rank;
    }

    std::string Ship::getCaptainName() const {
        return captain_name;
    }

    void Ship::setCaptainName(const std::string& cname) {
        if (cname.empty()) {
            throw std::invalid_argument("Captain name cannot be empty");
        }
        captain_name = cname;
    }

    double Ship::getSpeed() const {
        return speed;
    }

    void Ship::setSpeed(double s) {
        if (s < 0) {
            throw std::invalid_argument("Speed cannot be negative");
        }
        if (s > max_possible_speed) {
            throw std::invalid_argument("Speed exceeds maximum possible");
        }
        speed = s;
    }

    int Ship::getDurability() const {
        return durability;
    }

    void Ship::setDurability(int d) {
        if (d < 0) {
            throw std::invalid_argument("Durability cannot be negative");
        }
        durability = d;
    }

    double Ship::getCost() const {
        return cost;
    }

    void Ship::setCost(double c) {
        if (c < 0) {
            throw std::invalid_argument("Cost cannot be negative");
        }
        cost = c;
    }

    // *Методы работы с координатами

    ship::coordinate Ship::getCurrentCoordinates() const {
        return current_coordinates;
    }

    void Ship::setCurrentCoordinates(const ship::coordinate &coordinates) {
        current_coordinates = coordinates;
    }

    std::optional<ship::coordinate> Ship::getDestinationCoordinates() const {
        return destination_coordinates; // Если пусто, вернет std::nullopt
    }

    void Ship::setDestinationCoordinates(const ship::coordinate &coordinates) {
        destination_coordinates = coordinates;
    }

    // Улучшенный конструктор с валидацией
    Ship::Ship(IShip::shiptype type, const std::string &name,
               const std::string &captain_rank, const std::string &captain_name,
               double speed, int durability, double cost) {
        setShipType(type);
        setName(name);
        setCaptainRank(captain_rank);
        setCaptainName(captain_name);
        setSpeed(speed);
        setDurability(durability);
        setCost(cost);

        // Инициализация координат нулевыми значениями
        current_coordinates = {0.0, 0.0};
        destination_coordinates = {0.0, 0.0};
    }


    // *Работа с базовыми методами*

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
        return total_cost;
    }

    void Ship::move() {
        // Используем статическую переменную для отслеживания количества вызовов
        static int move_call_count = 0;
        const int move_interval_calls = 2; // Интервал обновления движения в количестве вызовов

        // Увеличиваем счетчик вызовов
        move_call_count++;

        // Если количество вызовов меньше интервала, выходим
        if (move_call_count < move_interval_calls) {
            return;
        }

        // Сбрасываем счетчик после достижения интервала
        move_call_count = 0;

        auto destination = getDestinationCoordinates();
        if (!destination.has_value()) return;

        double distance = calculateDistance(destination.value(), getCurrentCoordinates());
        if (distance > getSpeed()) {
            double ratio = getSpeed() / distance;
            ship::coordinate new_pos = {
                    getCurrentCoordinates().first +
                    (destination.value().first - getCurrentCoordinates().first) * ratio,
                    getCurrentCoordinates().second +
                    (destination.value().second - getCurrentCoordinates().second) * ratio
            };
            setCurrentCoordinates(new_pos);
        } else {
            setCurrentCoordinates(destination.value());
        }
    }

    void Ship::setDestination(const ship::coordinate &new_destination) {
        double max_distance = speed * 8;
        double required_distance = calculateDistance(new_destination, current_coordinates);
        if (required_distance > max_distance) {
            throw std::invalid_argument("Destination is too far");
        }

        destination_coordinates = new_destination;
    }

    double calculateDistance(const ship::coordinate& target_coordinates, const ship::coordinate& current_pos) {
        return std::sqrt(
                std::pow(target_coordinates.first - current_pos.first, 2) +
                std::pow(target_coordinates.second - current_pos.second, 2)
        );
    }



} // namespace acg