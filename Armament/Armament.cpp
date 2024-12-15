#include "Armament.h"

namespace acg {

    std::string Armament::getName() const {
        return name;
    }

    void Armament::setName(const std::string &name_) {
        name = name_;
    }

    Armament::ArmamentType Armament::getType() {
        return type;
    }

    void Armament::setType(Armament::ArmamentType type_) {
        type = type_;
    }

    std::string Armament::getAmmoName() const {
        return ammo_name;
    }

    void Armament::setAmmoName(const std::string &n) {
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
        damage = d;
    }

    double Armament::getRangeOfFire() const {
        return range_of_fire;
    }

    void Armament::setRangeOfFire(double r) {
        range_of_fire = r;
    }

    double Armament::getRateOfFire() const {
        return rate_of_fire;
    }

    void Armament::setRateOfFire(double r) {
        rate_of_fire = r;
    }

    int Armament::getMaxAmmoCapacity() const {
        return max_ammo_capacity;
    }

    void Armament::setMaxAmmoCapacity(int cap) {
        max_ammo_capacity = cap;
    }

    int Armament::getCurrentAmmo() const {
        return current_ammo;
    }

    void Armament::setCurrentAmmo(int ammo) {
        current_ammo = ammo;
    }

    double Armament::getReloadSpeed() const {
        return reload_speed;
    }

    void Armament::setReloadSpeed(double s) {
        reload_speed = s;
    }

    double Armament::getCost() const {
        return cost;
    }

    void Armament::setCost(double c) {
        cost = c;
    }

    void Armament::shoot() {
        if (!active || current_ammo <= 0) {
            return;
        }

        // Проверка времени между выстрелами
        static auto last_shot_time = std::chrono::steady_clock::now();
        auto current_time = std::chrono::steady_clock::now();
        auto time_diff = std::chrono::duration<double>
                (current_time - last_shot_time).count();

        if (time_diff < (1.0 / static_cast<double>(rate_of_fire))) {
            return; // Слишком рано для следующего выстрела
        }

        current_ammo--;
        last_shot_time = current_time;
    }


    void Armament::reload() {
        if (!active || current_ammo == max_ammo_capacity) {
            return;
        }

        static bool is_reloading = false;
        static auto reload_start_time = std::chrono::steady_clock::now(); // TODO нужен ли chrono?

        if (!is_reloading) {
            reload_start_time = std::chrono::steady_clock::now();
            is_reloading = true;
            return;
        }

        auto current_time = std::chrono::steady_clock::now();
        auto reload_time = std::chrono::duration_cast<std::chrono::seconds>
                (current_time - reload_start_time).count();

        if (reload_time >= static_cast<int64_t>(reload_speed)) { // Приведение к конкретному типу
            current_ammo = max_ammo_capacity;
            is_reloading = false;
        }
    }

} // namespace acg