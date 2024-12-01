#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "../Global.h"

namespace acg {
    class Aircraft {
    public:
        enum aircrafttype { FIGHTER, ATTACK };

    private:
        aircrafttype type;                   // Тип самолета: Истребитель, Штурмовик
        int damage;                          // Урон
        bool active;                         // Активен ли самолет
        int durability;                      // Прочность самолета
        double speed;                        // Скорость
        double fuel_consumption;             // Расход топлива (литров на километр)
        double fuel_capacity;                // Объем топлива (литров)
        double refuel_speed;                 // Скорость заправки (литров в секунду)
        double cost;                         // Стоимость самолета
        double attack_radius;                // Радиус атаки самолета (в километрах)

    public:
        Aircraft() = default;
        ~Aircraft() = default;

        // **Геттеры и сеттеры для каждого поля**
        [[nodiscard]] aircrafttype getType() const;                // Получить тип самолета
        void setType(aircrafttype t);                // Установить тип самолета
        [[nodiscard]] int getDamage() const ;                       // Получить количество урона
        void setDamage(int d);                       // Установить урон
        [[nodiscard]] bool getActive() const ;                       // Активен ли самолет
        void setActive(bool a);                      // Установить активность
        [[nodiscard]] int getDurability() const;                   // Получить прочность
        void setDurability(int d);                   // Установить прочность
        [[nodiscard]] double getSpeed() const;                     // Получить скорость
        void setSpeed(double s);                     // Установить скорость
        [[nodiscard]] double getFuelConsumption() const;           // Получить расход топлива
        void setFuelConsumption(double f);           // Установить расход топлива
        [[nodiscard]] double getFuelCapacity() const;              // Получить объем топлива
        void setFuelCapacity(double f);              // Установить объем топлива
        [[nodiscard]] double getRefuelSpeed() const;               // Получить скорость заправки
        void setRefuelSpeed(double r);               // Установить скорость заправки
        [[nodiscard]] double getCost() const;                      // Получить стоимость самолета
        void setCost(double c);                      // Установить стоимость
        [[nodiscard]] double getAttackRadius() const;              // Получить радиус атаки
        void setAttackRadius(double r);              // Установить радиус атаки

        // **Методы для самолета**
        void makeAttackRun(double distance);         // Сделать налет (расстояние)
        void receiveDamage(int damage_received);     // Получить повреждение
    };

} //namespace acg

#endif // AIRCRAFT_H


#pragma clang diagnostic pop