#include "Armament.h"

#include <utility>

namespace acg {

    Armament::Armament(std::string name, Armament::ArmamentType type, int damage, double range_of_fire,
                       double rate_of_fire, int max_ammo_capacity, double reload_speed, double cost)
            : name(std::move(name)),
              type(type),
              ammo_name("Standard"),
              active(false),
              damage(damage),
              range_of_fire(range_of_fire),
              rate_of_fire(rate_of_fire),
              max_ammo_capacity(max_ammo_capacity),
              current_ammo(max_ammo_capacity),
              reload_speed(reload_speed),
              cost(cost) {}

    std::string Armament::getName() const {
        return name;
    }

    void Armament::setName(const std::string &name_) {
        if (name_.empty()) {
            throw std::invalid_argument("Название оружия не может быть пустым");
        }
        name = name_;
    }

    Armament::ArmamentType Armament::getType() const {
        return type;
    }

    void Armament::setType(Armament::ArmamentType type_) {
        if (type_ != ArmamentType::LIGHT && type_ != ArmamentType::HEAVY) {
            throw std::invalid_argument("Неверный тип оружия");
        }
        type = type_;
    }

    std::string Armament::getAmmoName() const {
        return ammo_name;
    }

    void Armament::setAmmoName(const std::string &n) {
        if (n.empty()) {
            throw std::invalid_argument("Название боеприпасов не может быть пустым");
        }
        ammo_name = n;
    }

    bool Armament::getActive() const {
        return active;
    }

    void Armament::setActive(bool a) {
        active = a;
    }

    int Armament::getDamage() const {
        return damage;
    }

    void Armament::setDamage(int d) {
        if (d < 0) {
            throw std::invalid_argument("Урон не может быть отрицательным");
        }
        damage = d;
    }

    double Armament::getRangeOfFire() const {
        return range_of_fire;
    }

    void Armament::setRangeOfFire(double r) {
        if (r <= 0) {
            throw std::invalid_argument("Дальность стрельбы должна быть положительной");
        }
        range_of_fire = r;
    }

    double Armament::getRateOfFire() const {
        return rate_of_fire;
    }

    void Armament::setRateOfFire(double r) {
        if (r <= 0) {
            throw std::invalid_argument("Скорострельность должна быть положительной");
        }
        rate_of_fire = r;
    }

    int Armament::getMaxAmmoCapacity() const {
        return max_ammo_capacity;
    }

    void Armament::setMaxAmmoCapacity(int cap) {
        if (cap < 0) {
            throw std::invalid_argument("Ёмкость магазина не может быть отрицательной");
        }
        max_ammo_capacity = cap;
    }

    int Armament::getCurrentAmmo() const {
        return current_ammo;
    }

    void Armament::setCurrentAmmo(int ammo) {
        if (ammo < 0) {
            throw std::invalid_argument("Количество патронов не может быть отрицательным");
        }
        if (ammo > max_ammo_capacity) {
            throw std::invalid_argument("Превышена ёмкость магазина");
        }
        current_ammo = ammo;
    }

    double Armament::getReloadSpeed() const {
        return reload_speed;
    }

    void Armament::setReloadSpeed(double s) {
        if (s <= 0) {
            throw std::invalid_argument("Скорость перезарядки должна быть положительной");
        }
        reload_speed = s;
    }

    double Armament::getCost() const {
        return cost;
    }

    void Armament::setCost(double c) {
        if (c < 0) {
            throw std::invalid_argument("Стоимость не может быть отрицательной");
        }
        cost = c;
    }

    void Armament::shoot() {
        if (!active || current_ammo <= 0) {
            return;
        }
        static int shoot_count = 0;
        int max_shoots_per_second = static_cast<int>(rate_of_fire);
        shoot_count++;
        if (shoot_count <= max_shoots_per_second) {
            current_ammo--;
            shoot_count = 0;
        }
    }

    void Armament::reload() {
        if (!active || current_ammo == max_ammo_capacity) {
            return;
        }
        static bool is_reloading = false;
        static int reload_count = 0;

        // Рассчитываем количество секунд, необходимых для полной перезарядки
        int reload_time = static_cast<int>(reload_speed);

        if (!is_reloading) {
            is_reloading = true;
            reload_count = 0;  // Сбрасываем счетчик перезарядки
            return;
        }

        if (reload_count >= reload_time) {
            current_ammo = max_ammo_capacity;
            is_reloading = false;
        } else {
            reload_count++;
        }
    }

    bool Armament::operator==(const Armament& other) const {
        return name == other.name &&
               type == other.type &&
               ammo_name == other.ammo_name &&
               active == other.active &&
               damage == other.damage &&
               range_of_fire == other.range_of_fire &&
               rate_of_fire == other.rate_of_fire &&
               max_ammo_capacity == other.max_ammo_capacity &&
               current_ammo == other.current_ammo &&
               reload_speed == other.reload_speed &&
               cost == other.cost;
    }

} // namespace acg