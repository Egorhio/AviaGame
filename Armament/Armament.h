#pragma once

#include <chrono>
#include <string>

namespace acg {

/**
 * @brief Класс, представляющий вооружение корабля.
 *
 * Класс Armament инкапсулирует основные характеристики и поведение
 * вооружения, включая тип, урон, дальность, скорострельность и другие параметры.
 */
    class Armament {
    public:
        /**
         * @brief Перечисление типов вооружения.
         */
        enum class ArmamentType {
            LIGHT,  ///< Легкое вооружение
            HEAVY   ///< Тяжелое вооружение
        };

    private:
        std::string name = "Unknown";         ///< Название оружия
        ArmamentType type = ArmamentType::LIGHT; ///< Тип оружия
        std::string ammo_name = "Standard";   ///< Название боеприпаса
        bool active = false;                  ///< Статус активности оружия
        int damage = 0;                       ///< Урон, наносимый оружием
        double range_of_fire = 0.0;           ///< Дальность стрельбы
        double rate_of_fire = 0.0;            ///< Скорострельность
        int max_ammo_capacity = 0;            ///< Максимальная емкость боеприпасов
        int current_ammo = 0;                 ///< Текущее количество боеприпасов
        double reload_speed = 0.0;            ///< Скорость перезарядки
        double cost = 0.0;                    ///< Стоимость оружия

    public:
        /**
         * @brief Конструктор по умолчанию.
         */
        Armament() = default;

        /**
         * @brief Получить название оружия.
         * @return Название оружия.
         */
        [[nodiscard]] std::string getName() const;

        /**
         * @brief Установить название оружия.
         * @param name_ Новое название.
         */
        void setName(const std::string& name_);

        /**
         * @brief Получить тип оружия.
         * @return Тип оружия.
         */
        [[nodiscard]] ArmamentType getType() const;

        /**
         * @brief Установить тип оружия.
         * @param type_ Новый тип.
         */
        void setType(ArmamentType type_);

        /**
         * @brief Получить название боеприпаса.
         * @return Название боеприпаса.
         */
        [[nodiscard]] std::string getAmmoName() const;

        /**
         * @brief Установить название боеприпаса.
         * @param n Новое название боеприпаса.
         */
        void setAmmoName(const std::string& n);

        /**
         * @brief Проверить, активно ли оружие.
         * @return Статус активности.
         */
        [[nodiscard]] bool getActive() const;

        /**
         * @brief Установить активность оружия.
         * @param a Новый статус активности.
         */
        void setActive(bool a);

        /**
         * @brief Получить урон оружия.
         * @return Урон.
         */
        [[nodiscard]] int getDamage() const;

        /**
         * @brief Установить урон оружия.
         * @param d Новый урон.
         */
        void setDamage(int d);

        /**
         * @brief Получить дальность стрельбы.
         * @return Дальность стрельбы.
         */
        [[nodiscard]] double getRangeOfFire() const;

        /**
         * @brief Установить дальность стрельбы.
         * @param r Новая дальность.
         */
        void setRangeOfFire(double r);

        /**
         * @brief Получить скорострельность.
         * @return Скорострельность.
         */
        [[nodiscard]] double getRateOfFire() const;

        /**
         * @brief Установить скорострельность.
         * @param r Новая скорострельность.
         */
        void setRateOfFire(double r);

        /**
         * @brief Получить максимальную емкость боеприпасов.
         * @return Максимальная емкость.
         */
        [[nodiscard]] int getMaxAmmoCapacity() const;

        /**
         * @brief Установить максимальную емкость боеприпасов.
         * @param cap Новая емкость.
         */
        void setMaxAmmoCapacity(int cap);

        /**
         * @brief Получить текущее количество боеприпасов.
         * @return Текущее количество боеприпасов.
         */
        [[nodiscard]] int getCurrentAmmo() const;

        /**
         * @brief Установить текущее количество боеприпасов.
         * @param ammo Новое количество боеприпасов.
         */
        void setCurrentAmmo(int ammo);

        /**
         * @brief Получить скорость перезарядки.
         * @return Скорость перезарядки.
         */
        [[nodiscard]] double getReloadSpeed() const;

        /**
         * @brief Установить скорость перезарядки.
         * @param s Новая скорость перезарядки.
         */
        void setReloadSpeed(double s);

        /**
         * @brief Получить стоимость оружия.
         * @return Стоимость.
         */
        [[nodiscard]] double getCost() const;

        /**
         * @brief Установить стоимость оружия.
         * @param c Новая стоимость.
         */
        void setCost(double c);

        /**
         * @brief Произвести выстрел.
         *
         * Уменьшает количество боеприпасов при успешном выстреле.
         */
        void shoot();

        /**
         * @brief Перезарядить оружие.
         *
         * Восполняет боезапас до максимального значения.
         */
        void reload();
    };

} // namespace acg