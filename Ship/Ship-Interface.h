/**
 * @file Ship-Interface.h
 * @brief Интерфейс для всех типов кораблей в игре
 * @author Egor Volkov
 */
#pragma once

#include "Armament.h"
#include "Aircraft.h"
#include <vector>
#include <map>
#include <valarray>
#include <optional>

namespace acg {

    namespace ship {
        /**
         * @struct AmmoInfo
         * @brief Структура для хранения информации о боеприпасах, хранящихся на складе
         */
        struct AmmoInfo {
            int quantity = 0;         ///< Текущее количество боеприпасов
            double size = 0;          ///< Размер единицы боеприпаса
            double cost = 0;          ///< Стоимость единицы боеприпаса

            AmmoInfo() = default;

            /**
             * @brief Конструктор с параметрами
             * @param q Количество боеприпасов
             * @param s Размер единицы
             * @param c Стоимость единицы
             */
            AmmoInfo(int q, double s, double c) : quantity(q), size(s), cost(c) {}
        };

        /// @typedef armvector Вектор вооружения корабля
        using armvector = std::vector<Armament>;

        /// @typedef ammomap Карта боеприпасов (название -> информация)
        using ammomap = std::map<std::string, AmmoInfo>;

        /// @typedef coordinate Координаты в двумерном пространстве
        using coordinate = std::pair<double, double>;

        /// @typedef airvector Вектор авиации с их координатами
        using airvector = std::vector<std::pair<Aircraft, coordinate>>;
    } // namespace ship

    /**
     * @brief Базовый интерфейс для всех типов кораблей
     *
     * Определяет основной функционал, который должен быть реализован всеми типами кораблей
     */
    class IShip {
    public:
        /**
         * @brief Виртуальный деструктор
         */
        virtual ~IShip() = default;

        /**
         * @brief Перечисление типов кораблей
         */
        enum class shiptype {
            CRUISER,         /**< Крейсер */
            AIRCRAFTCARRIER, /**< Авианосец */
            AVIATORCRUISER  /**< Авианесущий крейсер */
        };

        /**
         * @brief Рассчитывает общую стоимость корабля
         * @return Общая стоимость корабля с учетом вооружения и состояния
         */
        [[nodiscard]] virtual double calculateTotalCost() const = 0;

        /**
         * @brief Устанавливает новую точку назначения для корабля
         * @param new_destination Новые координаты назначения
         * @throw std::invalid_argument если точка назначения недостижима
         */
        virtual void setDestination(const ship::coordinate &new_destination) = 0;

        /**
         * @brief Перемещает корабль к точке назначения
         *
         * Реализует движение корабля с учетом его скорости и текущего положения
         */
        virtual void move() = 0;
    };

} // namespace acg
