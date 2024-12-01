#include "Armament.h"

namespace acg {

    std::string Armament::getName() const {
        return name;
    }

    void Armament::setName(const std::string &name_) {
        name = name_;
    }

    Armament::armamenttype Armament::getType() {
        return type;
    }

    void Armament::setType(Armament::armamenttype type_) {
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
        //TODO выстрелить
    }

    void Armament::reload() {
        //TODO перезарядить
    }


} // namespace acg