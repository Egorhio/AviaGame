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
        static auto last_move_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();

        // Используем более безопасный способ получения продолжительности
        auto elapsed = static_cast<double>
                (std::chrono::duration_cast<std::chrono::milliseconds>
                        (current_time - last_move_time).count()) / 1000.0;

        double distance = speed * elapsed;

        // Убедимся, что используем double для вычислений
        double total_distance = std::sqrt(
                std::pow(destination_coordinates.first - current_coordinates.first, 2.0) +
                std::pow(destination_coordinates.second - current_coordinates.second, 2.0)
        );

        if (distance >= total_distance) {
            current_coordinates = destination_coordinates;
        } else {
            double ratio = distance / total_distance;
            current_coordinates.first +=
                    static_cast<double>(destination_coordinates.first - current_coordinates.first) * ratio;
            current_coordinates.second +=
                    static_cast<double>(destination_coordinates.second - current_coordinates.second) * ratio;
        }

        last_move_time = current_time;
    }

    void Ship::setDestination(const ship::coordinate &new_destination) {
        using namespace std::chrono_literals;
        auto max_time = 24h;
        double max_distance = speed * static_cast<double>(
                std::chrono::duration_cast<std::chrono::hours>(max_time).count()
        );

        double required_distance = std::sqrt(
                std::pow(new_destination.first - current_coordinates.first, 2.0) +
                std::pow(new_destination.second - current_coordinates.second, 2.0)
        );

        if (required_distance > max_distance) {
            throw std::invalid_argument("Destination is too far");
        }

        destination_coordinates = new_destination;
    }



} // namespace acg