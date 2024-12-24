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
         * @brief Выполнить атаку бомбардировщиками по заданным координатам
         * @param target_coordinates Координаты цели для атаки
         */
        virtual void bomberAttack(const ship::coordinate& target_coordinates) = 0;

        /**
         * @brief Выполнить атаку истребителями по вражеской авиации
         * @param enemy_aircraft Вектор вражеских самолетов и их координат
         */
        virtual void interceptorAttack(const ship::airvector& enemy_aircraft) = 0;
    };

} //namespace acg
