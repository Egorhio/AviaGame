#pragma once
#include <future>
#include "Table.h"
#include "Cruiser/Cruiser.h"
#include "AircraftCarrier/AircraftCarrier.h"
#include "AviatorCruiser/AviatorCruiser.h"
#include <thread>

#include <iostream>

namespace acg {

    /**
     * @enum MissionError
     * @brief Перечисление возможных ошибок в миссии
     */
    enum class MissionError {
        SUCCESS,                ///< Успешное выполнение
        INSUFFICIENT_FUNDS,     ///< Недостаточно средств
        EMPTY_CALLSIGN,         ///< Пустой позывной
        SHIP_NOT_FOUND,         ///< Корабль не найден
        MAX_SHIPS_REACHED,      ///< Достигнуто максимальное количество кораблей
        INVALID_SHIP_TYPE,      ///< Неверный тип корабля
        AIRCRAFT_NOT_FOUND,     ///< Самолет не найден
        WEAPON_NOT_FOUND,       ///< Оружие не найдено
        STORAGE_FULL,           ///< Хранилище заполнено
        VOID_LIST               ///< Пустой список
    };

    /**
     * @class Mission
     * @brief Класс, представляющий миссию
     * @date 2024
     */
    class Mission {
    private:
        ShipTable<Ship> shipGroupTable; ///< Таблица судов группы
        std::string commander = "Unnamed"; ///< Командующий миссией
        int max_ships = 0; ///< Максимальное возможное количество кораблей в группе
        double budget = 0.0; ///< Бюджет на миссию
        double spent_sum = 0.0; ///< Потраченная сумма денег
        double damage_per_group = 0.0; ///< Ущерб каждой группы
        double necessary_damage = 0.0; ///< Необходимый ущерб для победы
        double total_enemy_cost = 0.0; ///< Общая стоимость противника
        double saved_units_cost = 0.0; ///< Стоимость спасшихся единиц
        float size_baseA = 0.0; ///< Размер базы А
        float size_baseB = 0.0; ///< Размер базы Б
        ship::coordinate baseA_coordinates; ///< Координаты базы А
        ship::coordinate baseB_coordinates; ///< Координаты базы Б

    public:
        [[nodiscard]] size_t getCount() const {
            return shipGroupTable.getShipCount();
        }
        /**
         * @brief Конструктор по умолчанию
         */
        Mission() = default;

        /**
         * @brief Деструктор
         */
        ~Mission() = default;

        /**
         * @brief Конструктор с параметрами
         * @param commander Имя командующего
         * @param maxShips Максимальное количество кораблей
         * @param budget Бюджет миссии
         */
        Mission(std::string commander, int maxShips, double budget);

        // Методы доступа: геттеры

        /**
         * @brief Получает имя командующего
         * @return Имя командующего
         */
        [[nodiscard]] std::string getCommander() const;

        /**
         * @brief Получает максимальное количество кораблей
         * @return Максимальное количество кораблей
         */
        [[nodiscard]] int getMaxShips() const;

        /**
         * @brief Получает бюджет миссии
         * @return Бюджет миссии
         */
        [[nodiscard]] double getBudget() const;

        /**
         * @brief Получает потраченную сумму
         * @return Потраченная сумма
         */
        [[nodiscard]] double getSpentSum() const;

        /**
         * @brief Получает ущерб каждой группы
         * @return Ущерб каждой группы
         */
        [[nodiscard]] double getDamagePerGroup() const;

        /**
         * @brief Получает необходимый ущерб для победы
         * @return Необходимый ущерб для победы
         */
        [[nodiscard]] double getNecessaryDamage() const;

        /**
         * @brief Получает общую стоимость противника
         * @return Общая стоимость противника
         */
        [[nodiscard]] double getTotalEnemyCost() const;

        /**
         * @brief Получает стоимость спасшихся единиц
         * @return Стоимость спасшихся единиц
         */
        [[nodiscard]] double getSavedUnitsCost() const;

        /**
         * @brief Получает размер базы А
         * @return Размер базы А
         */
        [[nodiscard]] float getSizeBaseA() const;

        /**
         * @brief Получает размер базы Б
         * @return Размер базы Б
         */
        [[nodiscard]] float getSizeBaseB() const;

        /**
         * @brief Получает координаты базы А
         * @return Координаты базы А
         */
        [[nodiscard]] ship::coordinate getBaseACoordinates() const;

        /**
         * @brief Получает координаты базы Б
         * @return Координаты базы Б
         */
        [[nodiscard]] ship::coordinate getBaseBCoordinates() const;

        /**
         * @brief Получает оставшийся бюджет
         * @return Оставшийся бюджет
         */
        [[nodiscard]] double getRemainingBudget() const;

        // Методы для модификации: сеттеры

        /**
         * @brief Устанавливает имя командующего
         * @param name Имя командующего
         */
        void setCommander(const std::string& name);

        /**
         * @brief Устанавливает максимальное количество кораблей
         * @param maxShips Максимальное количество кораблей
         */
        void setMaxShips(int maxShips);

        /**
         * @brief Устанавливает бюджет миссии
         * @param budget Бюджет миссии
         */
        void setBudget(double budget);

        /**
         * @brief Устанавливает потраченную сумму
         * @param spent Потраченная сумма
         */
        void setSpentSum(double spent);

        /**
         * @brief Устанавливает ущерб каждой группы
         * @param damage Ущерб каждой группы
         */
        void setDamagePerGroup(double damage);

        /**
         * @brief Устанавливает необходимый ущерб для победы
         * @param damage Необходимый ущерб для победы
         */
        void setNecessaryDamage(double damage);

        /**
         * @brief Устанавливает общую стоимость противника
         * @param cost Общая стоимость противника
         */
        void setTotalEnemyCost(double cost);

        /**
         * @brief Устанавливает стоимость спасшихся единиц
         * @param cost Стоимость спасшихся единиц
         */
        void setSavedUnitsCost(double cost);

        /**
         * @brief Устанавливает размер базы А
         * @param size Размер базы А
         */
        void setSizeBaseA(float size);

        /**
         * @brief Устанавливает размер базы Б
         * @param size Размер базы Б
         */
        void setSizeBaseB(float size);

        /**
         * @brief Устанавливает координаты базы А
         * @param coordinates Координаты базы А
         */
        void setBaseACoordinates(const ship::coordinate& coordinates);

        /**
         * @brief Устанавливает координаты базы Б
         * @param coordinates Координаты базы Б
         */
        void setBaseBCoordinates(const ship::coordinate& coordinates);

        // Методы управления суднами

        /**
         * @brief Покупает корабль
         * @param call_sign Позывной корабля
         * @param ship Указатель на корабль
         * @return Код ошибки
         */
        MissionError buyShip(const std::string& call_sign, Ship* ship);

        /**
         * @brief Продает корабль
         * @param call_sign Позывной корабля
         * @return Код ошибки
         */
        MissionError sellShip(const std::string& call_sign);

        /**
         * @brief Перемещает самолет с одного корабля на другой
         * @param plane Указатель на самолет
         * @param from_ship Позывной исходного корабля
         * @param to_ship Позывной целевого корабля
         * @return Код ошибки
         */
        MissionError transferPlane(Aircraft* plane, const std::string& from_ship, const std::string& to_ship);

        // Методы управления ресурсами

        /**
         * @brief Покупает самолет для корабля
         * @param call_sign Позывной корабля
         * @param plane Указатель на самолет
         * @return Код ошибки
         */
        MissionError buyPlaneForShip(const std::string& call_sign, Aircraft* plane);

        /**
         * @brief Продает самолет с корабля
         * @param call_sign Позывной корабля
         * @param plane Указатель на самолет
         * @return Код ошибки
         */
        MissionError sellPlaneFromShip(const std::string& call_sign, Aircraft* plane);

        /**
         * @brief Покупает оружие для корабля
         * @param ship_call_sign Позывной корабля
         * @param weapon Указатель на оружие
         * @return Код ошибки
         */
        MissionError buyWeaponForShip(const std::string& ship_call_sign, Armament* weapon);

        /**
         * @brief Продает оружие с корабля
         * @param ship_call_sign Позывной корабля
         * @param weapon Указатель на оружие
         * @return Код ошибки
         */
        MissionError sellWeaponFromShip(const std::string& ship_call_sign, Armament* weapon);

        // Боевые действия

        /**
         * @brief Уничтожает корабль
         * @param call_sign Позывной корабля
         * @return Код ошибки
         */
        MissionError destroyShip(const std::string& call_sign);

        /**
         * @brief Помечает врага как достигнутого
         * @param enemy_call_sign Позывной врага
         * @return Код ошибки
         */
        MissionError markEnemyAsReached(const std::string& enemy_call_sign);

        /**
         * @brief Получает список самолетов
         * @param aircarrier Указатель на авианосец
         * @param aviator Указатель на авиатор
         * @param current_aircraft Текущий вектор самолетов
         * @return true, если список получен успешно
         */
        static bool getAircraftList(IAircraftCarrier *aircarrier, IAviatorCruiser *aviator, ship::airvector &current_aircraft);

        /**
         * @brief Получает список вооружений
         * @param cruiser Указатель на крейсер
         * @param aviator Указатель на авиатор
         * @param current_aircraft Текущий вектор вооружений
         * @return true, если список получен успешно
         */
        static bool getArmamentList(ICruiser *cruiser, IAviatorCruiser *aviator, ship::armvector &current_aircraft);

        /**
         * @brief Проверка бюджета
         * @param cost Стоимость для проверки
         * @return true, если бюджет достаточен
         */
        [[nodiscard]] bool checkBudget(double cost) const;
    };

} // namespace acg