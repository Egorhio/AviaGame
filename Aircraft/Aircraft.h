#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "../Global.h"

namespace acg {
    class Aircraft {
    public:
        enum class AircraftType { FIGHTER, ATTACK };
    private:
        AircraftType type = AircraftType::FIGHTER;  // Тип самолета
        int damage = 0;                             // Урон
        bool active = false;                        // Активен ли самолет
        int durability = 100;                       // Прочность самолета
        double speed = 0.0;                         // Скорость
        double fuel_consumption = 0.0;              // Расход топлива (литров на километр)
        double fuel_capacity = 0.0;                 // Объем топлива (литров)
        double refuel_speed = 0.0;                  // Скорость заправки (литров в секунду)
        double cost = 0.0;                          // Стоимость самолета
        double attack_radius = 0.0;                 // Радиус атаки самолета (в километрах)

    public:
        Aircraft() = default;
        ~Aircraft() = default;

        // **Геттеры и сеттеры для каждого поля**
        [[nodiscard]] AircraftType getType() const;                // Получить тип самолета
        void setType(AircraftType t);                // Установить тип самолета
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
        [[nodiscard]] double getEffectiveAttackRadius() const;     // Получить радиус атаки (эффективный) через другие методы
    };

} //namespace acg

#endif // AIRCRAFT_H