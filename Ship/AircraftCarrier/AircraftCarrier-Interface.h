/**
 * @file IAircraftCarrier.h
 * @brief Интерфейс для авианосцев
 * @author Egor Volkov
 * @date 2024
 */

#pragma once

#include "Ship.h"

namespace acg {

    /**
     * @class IAircraftCarrier
     * @brief Интерфейс, определяющий базовый функционал авианосца
     *
     * Определяет основные методы для работы с авиагруппой корабля,
     * включая атаки бомбардировщиками и истребителями
     */
    class IAircraftCarrier {
    public:
        /**
         * @brief Виртуальный деструктор по умолчанию
         */
        virtual ~IAircraftCarrier() = default;

        /**
         * @brief Выполнить боевой заход бомбардировщиков по заданным координатам
         * @param target_coordinates Координаты цели для атаки
         * @return Суммарные разрушения, которые нанесли бомбардировщики
         */
        virtual double bomberAttack(const ship::coordinate& target_coordinates) = 0;

        /**
         * @brief Выполнить атаку истребителями по вражеской авиации
         * @param enemy_aircraft Вектор вражеских самолетов и их координат
         */
        virtual void interceptorAttack(const ship::airvector& enemy_aircraft) = 0;

        virtual void modifyAircrafts(const ship::airvector& updated_aircrafts) = 0;

        [[nodiscard]] virtual ship::airvector getAircrafts() const = 0;

        [[nodiscard]] virtual int getMaxAircraftCapacity() const = 0;

        /**
         * @brief Установить максимальную вместимость самолетов
         * @param max_cap Новая максимальная вместимость
         * @throw std::invalid_argument если значение отрицательное
         */
        virtual void setMaxAircraftCapacity(int max_cap) = 0;
    };

} //namespace acg
