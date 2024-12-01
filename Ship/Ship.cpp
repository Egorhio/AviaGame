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
        speed = s;
    }

    int Ship::getDurability() const {
        return durability;
    }

    void Ship::setDurability(int d) {
        durability = d;
    }

    double Ship::getCost() const {
        return cost; // ? суммарная стоимость корабля?
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


    // *Работа с базовыми методами
    // *
    void Ship::move() {
        current_coordinates = destination_coordinates;
        // TODO Более сложная логика передвижения
    }

    void Ship::receiveDamage(int damage) {
        durability -= damage;
        if (durability < 0) durability = 0;
        // TODO Более сложная логика получения дамага
    }

    Ship::Ship(const std::string& name, double cost) {
        this->name = name;
        this->cost = cost;
    }

} // namespace acg