//
// Created by myDesk on 20.11.2024.
//

#include "Aircraft.h"

namespace acg {

    Aircraft::aircrafttype Aircraft::getType() const {
        return type;
    }

    void Aircraft::setType(Aircraft::aircrafttype t) {
        type = t;
    }

    int Aircraft::getDamage() const {
        return damage;
    }

    void Aircraft::setDamage(int d) {
        damage = d;
    }

    bool Aircraft::getActive() const {
        return active;
    }

    void Aircraft::setActive(bool a) {
        active = a;
    }

    int Aircraft::getDurability() const {
        return durability;
    }

    void Aircraft::setDurability(int d) {
        durability = d;
    }

    double Aircraft::getSpeed() const {
        return speed;
    }

    void Aircraft::setSpeed(double s) {
        speed = s;
    }

    double Aircraft::getFuelConsumption() const {
        return fuel_consumption;
    }

    void Aircraft::setFuelConsumption(double f) {
        fuel_consumption = f;
    }

    double Aircraft::getFuelCapacity() const {
        return fuel_capacity;
    }

    void Aircraft::setFuelCapacity(double f) {
        fuel_capacity = f;
    }

    double Aircraft::getRefuelSpeed() const {
        return refuel_speed;
    }

    void Aircraft::setRefuelSpeed(double r) {
        refuel_speed = r;
    }

    double Aircraft::getCost() const {
        return cost;
    }

    void Aircraft::setCost(double c) {
        cost = c;
    }

    double Aircraft::getAttackRadius() const {
        return attack_radius; //todo через другие методы
    }

    void Aircraft::setAttackRadius(double r) {
        attack_radius = r;
    }

    void Aircraft::makeAttackRun(double distance) {
        // TODO Сделать налет
    }

    void Aircraft::receiveDamage(int damage_received) {
        // TODO Полученный урон
    }
} // acg