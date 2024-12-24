#include "Armament.h"

namespace acg {

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

        // Используем статическую переменную, но инициализируем ее только один раз
        static bool first_shoot = true;
        static auto last_shot_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();
        auto time_diff = std::chrono::duration_cast<std::chrono::duration<double>>(
                current_time - last_shot_time).count();

        if (time_diff >= (1.0 / rate_of_fire) || first_shoot) {
            current_ammo--;
            last_shot_time = current_time;
        }
        first_shoot = false;
    }

    void Armament::reload() {
        if (!active || current_ammo == max_ammo_capacity) {
            return;
        }
        static bool is_reloading = false;
        static auto reload_start_time = std::chrono::steady_clock::now();
        if (!is_reloading) {
            reload_start_time = std::chrono::steady_clock::now();
            is_reloading = true;
            return;
        }
        auto current_time = std::chrono::steady_clock::now();
        auto reload_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - reload_start_time).count();
        if (reload_time >= static_cast<int64_t>(reload_speed)) {
            current_ammo = max_ammo_capacity;
            is_reloading = false;
        }
    }
} // namespace acg