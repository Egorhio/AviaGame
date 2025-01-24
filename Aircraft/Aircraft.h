#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

namespace acg {

    /**
     * @brief Класс, представляющий самолет
     * @details Класс содержит основные характеристики и функциональность самолета,
     * включая тип, урон, прочность, скорость и топливные характеристики
     */
    class Aircraft {
    public:
        /**
         * @brief Перечисление типов самолетов
         */
        enum class AircraftType {
            FIGHTER,    ///< Истребитель
            ATTACK      ///< Штурмовик
        };

    private:
        AircraftType type = AircraftType::FIGHTER;   ///< Тип самолета
        int damage = 0;                              ///< Урон самолета
        bool active = false;                         ///< Флаг активности самолета
        int durability = 100;                        ///< Прочность самолета
        double speed = 0.0;                          ///< Скорость самолета
        double fuel_consumption = 0.0;               ///< Расход топлива (л/км)
        double fuel_capacity = 0.0;                  ///< Объем топливного бака (л)
        double refuel_speed = 0.0;                   ///< Скорость заправки (л/с)
        double cost = 0.0;                           ///< Стоимость самолета
        double attack_radius = 0.0;                  ///< Радиус атаки (км)

    public:

        /**
         * @brief Конструктор с параметрами.
         * Создает объект Aircraft с заданными параметрами.
         * @throw std::invalid_argument Если какой-либо параметр имеет недопустимое значение.
         */
        Aircraft(AircraftType type, int damage, bool active, int durability,
                 double speed, double fuel_consumption, double fuel_capacity,
                 double refuel_speed, double cost, double attack_radius);

        /** @brief Конструктор по умолчанию */
        Aircraft() = default;

        /**
         * @brief Получить тип самолета
         * @return Тип самолета (FIGHTER/ATTACK)
         */
        [[nodiscard]] AircraftType getType() const;

        /**
         * @brief Установить тип самолета
         * @param t Новый тип самолета
         */
        void setType(AircraftType t);

        /**
         * @brief Получить количество урона
         * @return Урон самолета
         */
        [[nodiscard]] int getDamage() const;

        /**
         * @brief Установить урон самолета
         * @param d Новый урон
         */
        void setDamage(int d);

        /**
         * @brief Проверить, активен ли самолет
         * @return Статус активности
         */
        [[nodiscard]] bool getActive() const;

        /**
         * @brief Установить активность самолета
         * @param a Новый статус активности
         */
        void setActive(bool a);

        /**
         * @brief Получить прочность самолета
         * @return Прочность самолета
         */
        [[nodiscard]] int getDurability() const;

        /**
         * @brief Установить прочность самолета
         * @param d Новая прочность
         */
        void setDurability(int d);

        /**
         * @brief Получить скорость самолета
         * @return Скорость самолета
         */
        [[nodiscard]] double getSpeed() const;

        /**
         * @brief Установить скорость самолета
         * @param s Новая скорость
         */
        void setSpeed(double s);

        /**
         * @brief Получить расход топлива
         * @return Расход топлива (л/км)
         */
        [[nodiscard]] double getFuelConsumption() const;

        /**
         * @brief Установить расход топлива
         * @param f Новый расход топлива
         */
        void setFuelConsumption(double f);

        /**
         * @brief Получить объем топливного бака
         * @return Объем топливного бака (л)
         */
        [[nodiscard]] double getFuelCapacity() const;

        /**
         * @brief Установить объем топливного бака
         * @param f Новый объем топлива
         */
        void setFuelCapacity(double f);

        /**
         * @brief Получить скорость заправки
         * @return Скорость заправки (л/с)
         */
        [[nodiscard]] double getRefuelSpeed() const;

        /**
         * @brief Установить скорость заправки
         * @param r Новая скорость заправки
         */
        void setRefuelSpeed(double r);

        /**
         * @brief Получить стоимость самолета
         * @return Стоимость самолета
         */
        [[nodiscard]] double getCost() const;

        /**
         * @brief Установить стоимость самолета
         * @param c Новая стоимость
         */
        void setCost(double c);

        /**
         * @brief Получить радиус атаки
         * @return Радиус атаки (км)
         */
        [[nodiscard]] double getAttackRadius() const;

        /**
         * @brief Установить радиус атаки
         * @param r Новый радиус атаки
         */
        void setAttackRadius(double r);

        /**
         * @brief Выполнить атакующий налет
         * @param distance Дистанция до цели
         */
        void makeAttackRun(double distance);

        /**
         * @brief Получить урон
         * @param damage_received Количество получаемого урона
         */
        void receiveDamage(int damage_received);

        /**
         * @brief Рассчитать эффективный радиус атаки
         * @return Эффективный радиус атаки с учетом других параметров
         */
        [[nodiscard]] double getEffectiveAttackRadius() const;

        /**
        * @brief Оператор сравнения равенства для объектов Aircraft.
        *
        * Сравнивает текущий объект Aircraft с другим объектом Aircraft на равенство.
        * Два объекта считаются равными, если все их значимые поля совпадают.
        *
        * @param other Объект Aircraft, с которым происходит сравнение.
        * @return true, если объекты равны, иначе false.
        */
        bool operator==(const Aircraft& other) const;
    };

} // namespace acg