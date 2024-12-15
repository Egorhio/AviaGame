#include "Aircraft.h"

namespace acg {

    Aircraft::AircraftType Aircraft::getType() const {
        return type;
    }

    void Aircraft::setType(Aircraft::AircraftType t) {
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
        return attack_radius;
    }

    void Aircraft::setAttackRadius(double r) {
        attack_radius = r;
    }

    double Aircraft::getEffectiveAttackRadius() const {
        // Радиус атаки как функция от скорости и расхода топлива
        double radius_function = (speed * fuel_capacity) / (2 * fuel_consumption);
        // Учитываем состояние самолета (durability влияет на эффективный радиус)
        double condition_factor = durability / 100.0;
        // Получаемый радиус атаки - среднее значение между функциональным и радиусом состояния
        double final_radius = ((attack_radius * condition_factor) + radius_function) / 2.0;

        return final_radius;
    }

    void Aircraft::receiveDamage(int damage_received) {
        if (!active) return;
        durability -= damage_received;
        // Уменьшение стоимости самолёта пропорционально полученному урону
        double damage_factor = static_cast<double>(damage_received) / 100.0;
        cost *= (1.0 - damage_factor * 0.3); // Уменьшаем стоимость до 30% при максимальном уроне

        // Уменьшение скорости при повреждениях
        speed *= (1.0 - damage_factor * 0.2); // Уменьшаем скорость до 20% при максимальном уроне
        if (durability <= 0) {
            durability = 0;
            active = false;
            cost *= 0.1; // Остаточная стоимость как металлолом
            speed = 0;
        }
    }

    void Aircraft::makeAttackRun(double distance) {
        if (!active || distance > attack_radius) return;

        // Учитываем состояние самолёта при нанесении урона
        double condition_factor = durability / 100.0;
        damage = static_cast<int>(damage * condition_factor);

        // Расход топлива при атаке
        double fuel_spent = distance * fuel_consumption * (1.0 + (1.0 - condition_factor) * 0.3);

        // Износ от выполнения боевой задачи
        durability -= static_cast<int>(distance * 0.1);

        // Небольшое снижение стоимости от износа
        cost *= (1.0 - 0.01); // Уменьшение на 1% за вылет

        if (fuel_capacity >= fuel_spent) {
            fuel_capacity -= fuel_spent;
        } else {
            active = false;
        }

        // Проверка на критическое состояние
        if (durability <= 20) {
            speed *= 0.8; // Снижение скорости при критических повреждениях
        }
    }
} // acg