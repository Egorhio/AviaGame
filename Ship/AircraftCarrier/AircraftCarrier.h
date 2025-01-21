/**
 * @file AircraftCarrier.h
 * @brief Определяет класс авианосца с возможностями управления авиагруппой
 * @author Egor Volkov
 * @date 2024
 */

#pragma once
#include <iostream>

#include "AircraftCarrier-Interface.h"

namespace acg {

    /** @typedef airothervector
     *  @brief Псевдоним для вектора указателей на самолеты
     */
    using airothervector = std::vector<Aircraft*>;

    /**
     * @class AircraftCarrier
     * @brief Класс, представляющий авианосец
     * @details Наследуется от базового класса Ship и интерфейса IAircraftCarrier
     */
    class AircraftCarrier : public Ship, public IAircraftCarrier {
    private:
        ship::airvector aircrafts{0};        ///< Вектор самолетов на борту
        int max_aircraft_capacity = 0;        ///< Максимальная вместимость самолетов

    public:
        AircraftCarrier(shiptype type, const std::string& name, const std::string& captain,
                        const std::string& owner, double speed, int durability, double cost)
                : Ship(type, name, captain, owner, speed, durability, cost) {
            // Дополнительная инициализация, если необходимо
        }

        AircraftCarrier(const AircraftCarrier&) = delete;  ///< Запрет копирования
        AircraftCarrier& operator=(const AircraftCarrier&) = delete;  ///< Запрет присваивания
        AircraftCarrier() = default;  ///< Конструктор по умолчанию
        ~AircraftCarrier() override = default;  ///< Виртуальный деструктор

        /**
         * @brief Получить максимальную вместимость самолетов
         * @return Максимальное количество самолетов
         */
        [[nodiscard]] int getMaxAircraftCapacity() const override;

        /**
         * @brief Установить максимальную вместимость самолетов
         * @param max_cap Новая максимальная вместимость
         * @throw std::invalid_argument если значение отрицательное
         */
        void setMaxAircraftCapacity(int max_cap);

        /**
         * @brief Получить информацию о самолетах на борту
         * @return Опциональный вектор самолетов
         */
        [[nodiscard]] std::optional<ship::airvector> getAircrafts() const override;

        /**
         * @brief Обновить список самолетов
         * @param updated_aircrafts Новый список самолетов
         * @throw std::invalid_argument если превышена максимальная вместимость
         */
        void modifyAircrafts(const ship::airvector& updated_aircrafts) override;

        /**
         * @brief Выполнить атаку бомбардировщиками
         * @param target_coordinates Координаты цели
         */
        void bomberAttack(const ship::coordinate& target_coordinates) override;

        /**
         * @brief Выполнить атаку истребителями
         * @param enemy_aircraft Вектор вражеских самолетов
         */
        void interceptorAttack(const ship::airvector& enemy_aircraft) override;

        /**
         * @brief Выполнить волны атак бомбардировщиков
         * @param available_bombers Доступные бомбардировщики
         * @param distance Дистанция до цели
         */
        static void executeAttackWaves(airothervector &available_bombers, double distance);

        /**
         * @brief Получить список готовых к бою истребителей
         * @return Вектор указателей на готовые истребители
         */
        airothervector getReadyFighters();

        /**
         * @brief Назначить цели истребителям
         * @param enemy_aircraft Вражеские самолеты
         * @param ready_fighters Готовые истребители
         */
        void assignTargetsToFighters(const ship::airvector &enemy_aircraft, airothervector &ready_fighters);

        /**
         * @brief Найти лучший истребитель для атаки
         * @param ready_fighters Список готовых истребителей
         * @param distance Дистанция до цели
         * @return Указатель на выбранный истребитель
         */
        static Aircraft* findBestFighter(const airothervector &ready_fighters, double distance);

        /**
         * @brief Рассчитать общую стоимость авианосца
         * @return Общая стоимость с учетом самолетов
         */
        [[nodiscard]] double calculateTotalCost() const override;

        /**
         * @brief Установить новую точку назначения
         * @param new_destination Новые координаты назначения
         */
        void setDestination(const ship::coordinate &new_destination) override;

        /**
         * @brief Выполнить движение авианосца
         */
        void move() override;
    };

} // namespace acg