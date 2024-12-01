#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef ARMAMENT_H
#define ARMAMENT_H

#include "../Global.h"

namespace acg {
    class Armament {
    public:
        enum armamenttype { LIGHT, HEAVY };

    private:
        std::string name;       // Название оружия
        armamenttype type;      // Тип оружия — использовать Armament::armamenttype
        std::string ammo_name;  // Название боеприпаса
        bool active;            // Активность оружия
        int damage;             // Урон, наносимый оружием
        double range_of_fire;   // Дальность стрельбы
        double rate_of_fire;    // Скорострельность (выстрелов в секунду)
        int max_ammo_capacity;  // Максимальное количество боеприпасов
        int current_ammo;       // Текущее количество боеприпасов
        double reload_speed;    // Скорость перезарядки (в секундах)
        double cost;            // Стоимость оружия

    public:
        // **Конструктор и деструктор**
        Armament() = default;
        ~Armament() = default;

        // **Геттеры и сеттеры для полей**
        [[nodiscard]] std::string getName() const; // Получить название
        void setName(const std::string& name_);        // Установить название
        [[nodiscard]] armamenttype getType() ;              // Получить тип оружия
        void setType(armamenttype type_);              // Установить тип оружия
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


#pragma clang diagnostic pop
