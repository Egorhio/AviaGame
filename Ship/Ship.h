#pragma once

#include "Ship-Interface.h"

/**
 * @file Ship.h
 * @brief Определяет базовый класс Ship для системы морских судов
 * @author Egor Volkov
 * @date 2024
 */

/**
 * @namespace acg
 * @brief Основное пространство имен для всех компонентов системы
 */
namespace acg {

/**
 * @class Ship
 * @brief Базовый класс для всех типов кораблей
 * @extends IShip
 */
    class Ship : public IShip {
    protected:
        shiptype ship_type = shiptype::CRUISER;               ///< Тип корабля
        std::string name = "Unnamed";                         ///< Название корабля
        std::string captain_rank = "Without rank";            ///< Звание капитана
        std::string captain_name= "Capitan without name";     ///< Имя капитана
        double speed = 0.0;                                   ///< Скорость корабля
        int durability = 0;                                   ///< Живучесть корабля
        ship::coordinate current_coordinates;                 ///< Текущие координаты
        ship::coordinate destination_coordinates;             ///< Координаты назначения
        double cost = 0.0;                                    ///< Стоимость корабля

    public:
        /** @brief Конструктор по умолчанию */
        Ship() = default;

        /** @brief Виртуальный деструктор */
        ~Ship() override = default;

        /** @brief Запрет копирования */
        Ship(const Ship&) = delete;
        Ship& operator=(const Ship&) = delete;

        /**
         * @brief Конструктор с параметрами
         * @param type Тип корабля
         * @param name Название корабля
         * @param captain_rank Звание капитана
         * @param captain_name Имя капитана
         * @param speed Скорость
         * @param durability Живучесть
         * @param cost Стоимость
         */
        Ship(shiptype type, const std::string& name, const std::string& captain_rank,
             const std::string& captain_name, double speed, int durability, double cost);

        // Геттеры и сеттеры

        /**
         * @brief Получает тип корабля
         * @return Тип корабля
         */
        [[nodiscard]] shiptype getShipType() const;

        /**
         * @brief Устанавливает тип корабля
         * @param type Новый тип корабля
         */
        void setShipType(shiptype type);

        /**
         * @brief Получает название корабля
         * @return Название корабля
         */
        [[nodiscard]] std::string getName() const;

        /**
         * @brief Устанавливает название корабля
         * @param name Новое название корабля
         */
        void setName(const std::string &name);

        /**
         * @brief Получает звание капитана
         * @return Звание капитана
         */
        [[nodiscard]] std::string getCaptainRank() const;

        /**
         * @brief Устанавливает звание капитана
         * @param rank Новое звание капитана
         */
        void setCaptainRank(const std::string &rank);

        /**
         * @brief Получает имя капитана
         * @return Имя капитана
         */
        [[nodiscard]] std::string getCaptainName() const;

        /**
         * @brief Устанавливает имя капитана
         * @param name Новое имя капитана
         */
        void setCaptainName(const std::string &name);

        /**
         * @brief Получает скорость корабля
         * @return Текущая скорость корабля
         */
        [[nodiscard]] double getSpeed() const;

        /**
         * @brief Устанавливает скорость корабля
         * @param speed Новая скорость корабля
         */
        void setSpeed(double speed);

        /**
         * @brief Получает живучесть корабля
         * @return Живучесть корабля
         */
        [[nodiscard]] int getDurability() const;

        /**
         * @brief Устанавливает живучесть корабля
         * @param durability Новая живучесть корабля
         */
        void setDurability(int durability);

        /**
         * @brief Получает стоимость корабля
         * @return Текущая стоимость корабля
         */
        [[nodiscard]] double getCost() const;

        /**
         * @brief Устанавливает стоимость корабля
         * @param cost Новая стоимость корабля
         */
        void setCost(double cost);

        // Методы работы с координатами

        /**
         * @brief Получает текущие координаты корабля
         * @return Текущие координаты корабля
         */
        [[nodiscard]] ship::coordinate getCurrentCoordinates() const;

        /**
         * @brief Устанавливает текущие координаты корабля
         * @param coordinates Новые текущие координаты
         */
        void setCurrentCoordinates(const ship::coordinate& coordinates);

        /**
         * @brief Получает координаты точки назначения корабля
         * @return Координаты точки назначения (если заданы)
         */
        [[nodiscard]] std::optional<ship::coordinate> getDestinationCoordinates() const;

        /**
         * @brief Устанавливает координаты точки назначения корабля
         * @param coordinates Новые координаты точки назначения
         */
        void setDestinationCoordinates(const ship::coordinate& coordinates);

        /**
         * @brief Перемещает корабль к точке назначения
         */
        void move() override;

        /**
         * @brief Обрабатывает получение урона кораблем
         * @param damage Величина урона
         */
        void receiveDamage(int damage);

        /**
         * @brief Рассчитывает полную стоимость корабля
         * @return Полная стоимость с учетом состояния
         */
        [[nodiscard]] double calculateTotalCost() const override;

        /**
         * @brief Устанавливает новую точку назначения
         * @param new_destination Новые координаты назначения
         */
        void setDestination(const ship::coordinate &new_destination) override;
    };

    /**
     * @brief Вычисляет расстояние между двумя точками
     * @param target_coordinates Целевые координаты
     * @param current_pos Текущие координаты
     * @return Расстояние между точками
     */
    double calculateDistance(const ship::coordinate &target_coordinates, const ship::coordinate &current_pos);

} // namespace acg