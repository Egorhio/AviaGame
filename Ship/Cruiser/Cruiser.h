#pragma once

#include "Cruiser-Interface.h"

namespace acg {

    /**
     * @brief Класс, представляющий крейсер - боевой корабль с вооружением
     * @details Наследуется от базового класса Ship и интерфейса ICruiser
     */
    class Cruiser : public Ship, public ICruiser {
    private:
        ship::armvector armament {0};        ///< Вектор вооружения корабля
        int max_armament_capacity = 0;        ///< Максимальная вместимость вооружения
        int storage_capacity = 0;             ///< Вместимость склада боеприпасов
        ship::ammomap ammo_storage;          ///< Хранилище боеприпасов (ключ - название)

    public:
        Cruiser(const Cruiser&) = delete;     ///< Запрет копирования
        Cruiser& operator=(const Cruiser&) = delete; ///< Запрет присваивания

        Cruiser() = default;                  ///< Конструктор по умолчанию
        ~Cruiser() override = default;        ///< Виртуальный деструктор

        /**
         * @brief Получить текущее вооружение крейсера
         * @return std::optional с вектором вооружения
         */
        [[nodiscard]] std::optional<ship::armvector> getArmament() const;

        /**
         * @brief Изменить вооружение крейсера
         * @param new_armament Новый вектор вооружения
         * @throw std::invalid_argument если превышена максимальная вместимость
         */
        void modifyArmament(const ship::armvector& new_armament);

        /**
         * @brief Получить информацию о конкретных боеприпасах
         * @param ammo_name Название боеприпасов
         * @return std::optional с информацией о боеприпасах
         */
        [[nodiscard]] std::optional<ship::AmmoInfo> getAmmoInfo(const std::string &ammo_name) const;

        /**
         * @brief Изменить информацию о боеприпасах
         * @param ammo_name Новая карта боеприпасов
         * @throw std::invalid_argument если превышена вместимость склада
         */
        void modifyAmmoInfo(const ship::ammomap& ammo_name);

        /**
         * @brief Получить максимальную вместимость вооружения
         * @return Максимальное количество единиц вооружения
         */
        [[nodiscard]] int getMaxArmamentCapacity() const;

        /**
         * @brief Установить максимальную вместимость вооружения
         * @param arm_capacity Новая максимальная вместимость
         * @throw std::invalid_argument если значение отрицательное
         */
        void setMaxArmamentCapacity(int arm_capacity);

        /**
         * @brief Получить вместимость склада боеприпасов
         * @return Вместимость склада
         */
        [[nodiscard]] int getStorageCapacity() const;

        /**
         * @brief Установить вместимость склада
         * @param st_c Новая вместимость
         * @throw std::invalid_argument если значение отрицательное
         */
        void setStorageCapacity(int st_c);

        /**
         * @brief Рассчитать доступное место на складе боеприпасов
         * @return Количество свободного места
         */
        [[nodiscard]] int calculateAvailableAmmoStorage() const override;

        /**
         * @brief Произвести выстрел по кораблю противника
         * @param target_coordinates Координаты цели
         */
        void fireAtShip(const ship::coordinate& target_coordinates) override;

        /**
         * @brief Перезарядить указанное оружие
         * @param weapon Оружие для перезарядки
         */
        void reloadWeapon(const Armament& weapon) override;

        /**
         * @brief Произвести выстрел по воздушным целям
         * @param enemy_aircraft Вектор вражеских самолетов
         */
        void fireAtAircraft(const ship::airvector& enemy_aircraft) override;

        /**
         * @brief Рассчитать общую стоимость крейсера
         * @return Общая стоимость с учетом вооружения и боеприпасов
         */
        [[nodiscard]] double calculateTotalCost() const override;

        /**
         * @brief Установить новую точку назначения
         * @param new_destination Новые координаты назначения
         */
        void setDestination(const ship::coordinate &new_destination) override;

        /**
         * @brief Переместить крейсер к точке назначения
         */
        void move() override;

        Cruiser(Ship::shiptype type, const std::string& name,
                         const std::string& captain_rank, const std::string& captain_name,
                         double speed, int durability, double cost,
                         int max_arm_capacity, int storage_cap);
    };
} // namespace acg