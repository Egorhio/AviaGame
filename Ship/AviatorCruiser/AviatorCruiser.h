/**
 * @file AviatorCruiser.h
 * @brief Определение класса AviatorCruiser, комбинирующего функциональность крейсера и авианосца.
 * @details Этот класс представляет собой гибридный корабль, который может выполнять функции как крейсера, так и авианосца.
 */

#pragma once

#include "AviatorCruiser-Interface.h"

namespace acg {

    /**
     * @typedef airothervector
     * @brief Псевдоним для вектора указателей на объекты Aircraft.
     */
    using airothervector = std::vector<Aircraft*>;

    /**
     * @class AviatorCruiser
     * @brief Класс гибридного корабля, сочетающего функции крейсера и авианосца.
     * @details Этот класс наследует функциональность от Ship, IAircraftCarrier и ICruiser, объединяя их возможности.
     */
    class AviatorCruiser : public Ship, public IAviatorCruiser {
    private:
        ship::armvector armament{0};       ///< Вектор вооружения корабля.
        int max_armament_capacity = 0;     ///< Максимальная вместимость вооружения.
        int storage_capacity = 0;          ///< Вместимость склада боеприпасов.
        ship::ammomap ammo_storage;        ///< Карта боеприпасов на складе, индексированная по названию.
        ship::airvector aircrafts{0};      ///< Вектор самолетов на борту.
        int max_aircraft_capacity = 0;     ///< Максимальная вместимость самолётов на борту.

    public:

        /**
         * @brief Конструктор класса AviatorCruiser
         * @param type Тип корабля (должен быть AVIATORCRUISER)
         * @param name Название корабля
         * @param captain_rank Звание капитана
         * @param captain_name Имя капитана
         * @param speed Скорость корабля
         * @param durability Прочность корабля
         * @param cost Базовая стоимость корабля
         * @param max_arm_capacity Максимальная вместимость вооружения
         * @param storage_cap Вместимость склада боеприпасов
         * @param max_aircraft_cap Максимальная вместимость самолётов
         * @throws std::invalid_argument если переданы некорректные параметры
         */
        AviatorCruiser(Ship::shiptype type, const std::string& name,
                                       const std::string& captain_rank, const std::string& captain_name,
                                       double speed, int durability, double cost,
                                       int max_arm_capacity, int storage_cap, int max_aircraft_cap);
        /**
         * @brief Конструктор по умолчанию.
         */
        AviatorCruiser() = default;

        /**
         * @brief Виртуальный деструктор.
         */
        ~AviatorCruiser() override = default;

        /**
         * @brief Запрещенный конструктор копирования.
         */
        AviatorCruiser(const AviatorCruiser&) = delete;

        /**
         * @brief Запрещенный оператор присваивания.
         */
        AviatorCruiser& operator=(const AviatorCruiser&) = delete;

        /**
         * @name Методы крейсерской части
         * @{
         */

        /**
         * @brief Получить текущее вооружение.
         * @return std::optional с вектором вооружения.
         */
        [[nodiscard]] std::optional<ship::armvector> getArmament() const override;

        /**
         * @brief Модифицировать вооружение корабля.
         * @param new_armament Новый вектор вооружения.
         * @throws std::invalid_argument если новое вооружение превышает максимальную вместимость.
         */
        void modifyArmament(const ship::armvector& new_armament) override;

        /**
         * @brief Получить информацию о боеприпасах по названию.
         * @param ammo_name Название боеприпасов.
         * @return std::optional с информацией о боеприпасах.
         */
        [[nodiscard]] std::optional<ship::AmmoInfo> getAmmoInfo(const std::string &ammo_name) const;

        /**
         * @brief Модифицировать информацию о боеприпасах.
         * @param ammo_name Карта боеприпасов.
         * @throws std::invalid_argument если общее количество боеприпасов превышает вместимость склада.
         */
        void modifyAmmoInfo(const ship::ammomap& ammo_name);

        /**
         * @brief Получить максимальную вместимость вооружения.
         * @return Максимальная вместимость.
         */
        [[nodiscard]] int getMaxArmamentCapacity() const;

        /**
         * @brief Установить максимальную вместимость вооружения.
         * @param arm_capacity Новая вместимость.
         * @throws std::invalid_argument если передано отрицательное значение.
         */
        void setMaxArmamentCapacity(int arm_capacity);

        /**
         * @brief Получить вместимость склада боеприпасов.
         * @return Вместимость склада.
         */
        [[nodiscard]] int getStorageCapacity() const;

        /**
         * @brief Установить вместимость склада.
         * @param st_c Новая вместимость.
         * @throws std::invalid_argument если передано отрицательное значение.
         */
        void setStorageCapacity(int st_c);

        /**
         * @brief Рассчитать доступное место для боеприпасов.
         * @return Количество свободного места на складе.
         */
        [[nodiscard]] int calculateAvailableAmmoStorage() const override;

        /**
         * @brief Произвести выстрел по кораблю противника.
         * @param target_coordinates Координаты цели.
         */
        void fireAtShip(const ship::coordinate& target_coordinates) override;

        /**
         * @brief Перезарядить указанное оружие.
         * @param weapon Оружие для перезарядки.
         */
        void reloadWeapon(const Armament& weapon) override;

        /**
         * @brief Выстрел по самолётам противника.
         * @param enemy_aircraft Вектор вражеских самолётов.
         */
        void fireAtAircraft(const ship::airvector& enemy_aircraft) override;

        /** @} */

        /**
         * @name Методы авианосной части
         * @{
         */

        /**
         * @brief Получить максимальную вместимость самолётов.
         * @return Максимальная вместимость самолётов.
         */
        [[nodiscard]] int getMaxAircraftCapacity() const override;

        /**
         * @brief Установить максимальную вместимость самолётов.
         * @param max_cap Новая вместимость самолётов.
         * @throws std::invalid_argument если передано отрицательное значение.
         */
        void setMaxAircraftCapacity(int max_cap);

        /**
         * @brief Получить информацию о самолётах.
         * @return std::optional с вектором самолётов.
         */
        [[nodiscard]] std::optional<ship::airvector> getAircrafts() const override;

        /**
         * @brief Модифицировать информацию о самолётах.
         * @param updated_aircrafts Новый вектор самолётов.
         * @throws std::invalid_argument если новое количество самолётов превышает максимальную вместимость.
         */
        void modifyAircrafts(const ship::airvector& updated_aircrafts) override;

        /**
         * @brief Выполнить атаку бомбардировщиками.
         * @param target_coordinates Координаты цели.
         */
        void bomberAttack(const ship::coordinate& target_coordinates) override;

        /**
         * @brief Выполнить атаку истребителями.
         * @param enemy_aircraft Вектор вражеских самолётов.
         */
        void interceptorAttack(const ship::airvector& enemy_aircraft) override;

        /** @} */

        /**
         * @name Общие методы корабля
         * @{
         */

        /**
         * @brief Переместить корабль в точку назначения.
         */
        void move() override;

        /**
         * @brief Рассчитать полную стоимость корабля.
         * @return Общая стоимость.
         */
        [[nodiscard]] double calculateTotalCost() const override;

        /**
         * @brief Установить новую точку назначения.
         * @param new_destination Новые координаты назначения.
         */
        void setDestination(const ship::coordinate &new_destination) override;

        /** @} */

        /**
         * @name Вспомогательные методы
         * @{
         */

        /**
         * @brief Выполнить волны атак бомбардировщиков.
         * @param available_bombers Доступные бомбардировщики.
         * @param distance Дистанция до цели.
         */
        static void executeAttackWaves(airothervector &available_bombers, double distance);

        /**
         * @brief Получить список готовых к бою истребителей.
         * @return Вектор указателей на готовые истребители.
         */
        airothervector getReadyFighters();

        /**
         * @brief Назначить цели истребителям.
         * @param enemy_aircraft Вражеские самолёты.
         * @param ready_fighters Готовые истребители.
         */
        void assignTargetsToFighters(const ship::airvector &enemy_aircraft, airothervector &ready_fighters);

        /**
         * @brief Найти лучший истребитель для атаки.
         * @param ready_fighters Список готовых истребителей.
         * @param distance Дистанция до цели.
         * @return Указатель на выбранный истребитель.
         */
        static Aircraft *findBestFighter(airothervector &ready_fighters, double distance);

        /** @} */
    };

} // namespace acg