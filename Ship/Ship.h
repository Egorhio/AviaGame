#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef SHIP_H
#define SHIP_H

#include "Ship-Interface.h"

namespace acg {

    class Ship : public IShip {
    protected:
        shiptype ship_type;                       // Тип корабля
        std::string name;                            // Название судна
        std::string captain_rank;                    // Звание капитана
        std::string captain_name;                    // Имя капитана
        double speed;                                // Скорость корабля
        int durability;                              // Живучесть судна
        ship::coordinate current_coordinates; // Текущие координаты корабля
        ship::coordinate destination_coordinates; // Координаты места следования
        double cost;                                 // Стоимость корабля

    public:
        Ship(const std::string& name, double cost);
        Ship() = default;
        ~Ship() override = default;

        // **Методы для любого судна**
        [[nodiscard]] shiptype getShipType() const override; // Получить тип корабля
        void setShipType(shiptype type) override;  // Установить тип корабля
        [[nodiscard]] std::string getName() const override; // Получить название судна
        void setName(const std::string &name) override; // Установить название судна
        [[nodiscard]] std::string getCaptainRank() const override; // Получить звание капитана
        void setCaptainRank(const std::string &rank) override; // Установить звание капитана
        [[nodiscard]] std::string getCaptainName() const override; // Получить имя капитана
        void setCaptainName(const std::string &name) override; // Установить имя капитана
        [[nodiscard]] double getSpeed() const override; // Получить текущую скорость
        void setSpeed(double speed) override; // Установить текущую скорость
        [[nodiscard]] int getDurability() const override; // Получить живучесть судна
        void setDurability(int durability) override; // Установить живучесть судна

        // *Методы работы с координатами*
        [[nodiscard]] ship::coordinate getCurrentCoordinates() const override; // Получить текущую точку
        void setCurrentCoordinates(const ship::coordinate& coordinates) override; // Установить текущую точку
        [[nodiscard]] ship::coordinate getDestinationCoordinates() const override; // Получить точку назначения
        void setDestinationCoordinates(const ship::coordinate& coordinates) override; // Установить точку назначения

        // *Дополнительные базовые методы*
        [[nodiscard]] double getCost() const override; // Вернуть стоимость корабля (суммарную)
        void setCost(double cost) override; // Установить стоимость для корабля
        void move() override; // Переместить корабль в точку назначения
        void receiveDamage(int damage) override; // Получить урон
    };

} // namespace acg

#endif // SHIP_H


#pragma clang diagnostic pop