#ifndef ARMAMENT_H
#define ARMAMENT_H

#include "../Global.h"
#include <chrono>

namespace acg {
    class Armament {
    public:
        enum class ArmamentType { LIGHT, HEAVY };

    private:
        std::string name = "Unknown";
        ArmamentType type = ArmamentType::LIGHT;
        std::string ammo_name = "Standard";
        bool active = false;
        int damage = 0;
        double range_of_fire = 0.0;
        double rate_of_fire = 0.0;
        int max_ammo_capacity = 0;
        int current_ammo = 0;
        double reload_speed = 0.0;
        double cost = 0.0;

    public:
        // **Конструктор и деструктор**
        Armament() = default;
        ~Armament() = default;

        // **Геттеры и сеттеры для полей**
        [[nodiscard]] std::string getName() const; // Получить название
        void setName(const std::string& name_);        // Установить название
        [[nodiscard]] ArmamentType getType() const;              // Получить тип оружия
        void setType(ArmamentType type_);              // Установить тип оружия
        [[nodiscard]] std::string getAmmoName() const;           // Получить название боеприпаса
        void setAmmoName(const std::string& n);    // Установить название боеприпаса
        [[nodiscard]] bool getActive() const;                     // Активность оружия
        void setActive(bool a);                    // Установить активность оружия
        [[nodiscard]] int getDamage() const;                     // Получить урон
        void setDamage(int d);                     // Установить урон
        [[nodiscard]] double getRangeOfFire() const;             // Получить дальность стрельбы
        void setRangeOfFire(double r);             // Установить дальность стрельбы
        [[nodiscard]] double getRateOfFire() const;              // Получить скорострельность
        void setRateOfFire(double r);              // Установить скорострельность
        [[nodiscard]] int getMaxAmmoCapacity() const;            // Получить максимальную емкость патронов
        void setMaxAmmoCapacity(int cap);          // Установить максимальную емкость
        [[nodiscard]] int getCurrentAmmo() const;                // Получить текущее количество боеприпасов
        void setCurrentAmmo(int ammo);             // Установить текущее количество боеприпасов
        [[nodiscard]] double getReloadSpeed() const;             // Получить скорость перезарядки
        void setReloadSpeed(double s);             // Установить скорость перезарядки
        [[nodiscard]] double getCost() const;                    // Получить стоимость оружия
        void setCost(double c);                    // Установить стоимость оружия

        // **Методы оружия**
        void shoot();                              // Выстрелить
        void reload();                             // Перезарядить
    };
} // namespace acg

#endif //ARMAMENT_H