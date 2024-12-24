/**
 * @file Cruiser-Interface.h
 * @brief Интерфейс для всех вооруженных кораблей в игре
 * @author Egor Volkov
 * @date 2024
 */
#pragma once

#include "Ship.h"

namespace acg {

    /**
     * @brief Интерфейс для крейсеров
     * @namespace acg
     */
    class ICruiser {
    public:
        /**
         * @brief Виртуальный деструктор по умолчанию
         */
        virtual ~ICruiser() = default;

        /**
         * @brief Рассчитывает доступное место для хранения боеприпасов
         * @return Количество доступных слотов для боеприпасов
         * @note Метод помечен как nodiscard для предотвращения игнорирования возвращаемого значения
         */
        [[nodiscard]] virtual int calculateAvailableAmmoStorage() const = 0;

        /**
         * @brief Производит выстрел по кораблю противника
         * @param target_coordinates Координаты цели для атаки
         */
        virtual void fireAtShip(const ship::coordinate& target_coordinates) = 0;

        /**
         * @brief Перезаряжает указанное оружие боеприпасами со склада
         * @param weapon Ссылка на оружие для перезарядки
         */
        virtual void reloadWeapon(const Armament& weapon) = 0;

        /**
         * @brief Производит выстрел по вражеской авиации
         * @param enemy_aircraft Вектор с информацией о вражеских самолетах
         */
        virtual void fireAtAircraft(const ship::airvector& enemy_aircraft) = 0;

        /**
         * @brief Получить текущее вооружение корабля
         * @return std::optional<ship::armvector> - вектор вооружения корабля
         *         nullopt если вооружение отсутствует
         * @nodiscard указывает, что возвращаемое значение не должно игнорироваться
         */
        [[nodiscard]] virtual std::optional<ship::armvector> getArmament() const = 0;

        /**
         * @brief Изменить вооружение корабля
         * @param new_armament Новый вектор вооружения
         * @throw std::invalid_argument если превышена максимальная вместимость вооружения
         */
        virtual void modifyArmament(const ship::armvector& new_armament) = 0;
    };

} // namespace acg
