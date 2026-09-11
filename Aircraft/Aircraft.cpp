#include "Aircraft.h"

namespace acg {

    Aircraft::Aircraft(AircraftType type, int damage, bool active, int durability,
                       double speed, double fuel_consumption, double fuel_capacity,
                       double refuel_speed, double cost, double attack_radius)
            : type(type)
            , damage(damage)
            , active(active)
            , durability(durability)
            , speed(speed)
            , fuel_consumption(fuel_consumption)
            , fuel_capacity(fuel_capacity)
            , refuel_speed(refuel_speed)
            , cost(cost)
            , attack_radius(attack_radius) {
        // Валидация параметров
        if (damage < 0) throw std::invalid_argument("Damage cannot be negative");
        if (durability < 0) throw std::invalid_argument("Durability cannot be negative");
        if (speed < 0) throw std::invalid_argument("Speed cannot be negative");
        if (fuel_consumption < 0) throw std::invalid_argument("Fuel consumption cannot be negative");
        if (fuel_capacity < 0) throw std::invalid_argument("Fuel capacity cannot be negative");
        if (refuel_speed < 0) throw std::invalid_argument("Refuel speed cannot be negative");
        if (cost < 0) throw std::invalid_argument("Cost cannot be negative");
        if (attack_radius < 0) throw std::invalid_argument("Attack radius cannot be negative");
    }

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
        if (d >= 0) damage = d;
        else throw std::invalid_argument("Урон самолёта не может быть отрицательным");
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
        if (d >= 0) durability = d;
        else throw std::invalid_argument("Прочность не может быть отрицательной");
    }

    double Aircraft::getSpeed() const {
        return speed;
    }

    void Aircraft::setSpeed(double s) {
        if (s >= 0) speed = s;
        else throw std::invalid_argument("Скорость не может быть отрицательной");
    }

    double Aircraft::getFuelConsumption() const {
        return fuel_consumption;
    }

    void Aircraft::setFuelConsumption(double f) {
        if (f >= 0) fuel_consumption = f;
        else throw std::invalid_argument("Расход топлива не может быть отрицательным");
    }

    double Aircraft::getFuelCapacity() const {
        return fuel_capacity;
    }

    void Aircraft::setFuelCapacity(double f) {
        if (f >= 0) fuel_capacity = f;
        else throw std::invalid_argument("Объем топлива не может быть отрицательным");
    }

    double Aircraft::getRefuelSpeed() const {
        return refuel_speed;
    }

    void Aircraft::setRefuelSpeed(double r) {
        if (r >= 0) refuel_speed = r;
        else throw std::invalid_argument("Скорость заправки отрицательная, норм?");
    }

    double Aircraft::getCost() const {
        return cost;
    }

    void Aircraft::setCost(double c) {
        if (c >= 0) cost = c;
        else throw std::invalid_argument("Стоимость отрицательная, норм?");
    }

    double Aircraft::getAttackRadius() const {
        return attack_radius;
    }

    void Aircraft::setAttackRadius(double r) {
        if (r >= 0) attack_radius = r;
        else throw std::invalid_argument("Радиус атаки отрицателен, норм?");
    }

    double Aircraft::getEffectiveAttackRadius() const {
        if (!active || durability <= 0) return 0.0;
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
        double damage_factor = static_cast<double>(damage_received) / 100.0;
        cost *= (1.0 - damage_factor * 0.3); // Уменьшаем стоимость до 30% при максимальном уроне
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

        double condition_factor = durability / 100.0;

        // Расход топлива при атаке (с учетом расстояния туда и обратно и состояния)
        double fuel_spent = distance * 2 * fuel_consumption * (1.0 + (1.0 - condition_factor) * 0.3);

        // Не хватает топлива на возврат — боевой заход невозможен
        if (fuel_capacity < fuel_spent) {
            active = false;
            return;
        }

        // Уменьшаем топливо (единственное место, где оно тратится за вылет)
        fuel_capacity -= fuel_spent;

        // Износ от выполнения боевой задачи
        durability -= static_cast<int>(distance * 0.1);
        if (durability <= 0) {
            durability = 0;
            active = false;
        }

        // Небольшое снижение стоимости от износа
        cost *= (1.0 - 0.01);

        // Проверка на критическое состояние
        if (durability > 0 && durability <= 20) {
            speed *= 0.8;
        }
    }

    bool Aircraft::operator==(const Aircraft& other) const {
        return type == other.type &&
               damage == other.damage &&
               active == other.active &&
               durability == other.durability &&
               speed == other.speed &&
               fuel_consumption == other.fuel_consumption &&
               fuel_capacity == other.fuel_capacity &&
               refuel_speed == other.refuel_speed &&
               cost == other.cost &&
               attack_radius == other.attack_radius;
    }

} // namespace acg