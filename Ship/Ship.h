#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef SHIP_H
#define SHIP_H

#include "Ship-Interface.h"

namespace acg {

    class Ship : public IShip {
    protected:
        shiptype ship_type = shiptype::CRUISER;
        std::string name = "Unnamed";
        std::string captain_rank = "None";
        std::string captain_name = "None";
        double speed = 0.0;
        int durability = 100;
        ship::coordinate current_coordinates = {0.0, 0.0};
        ship::coordinate destination_coordinates = {0.0, 0.0};
        double cost = 0.0;

    public:
        Ship() = default;
        ~Ship() override = default;

        Ship(const Ship&) = delete;
        Ship& operator=(const Ship&) = delete; // TODO нужна ли защита от копирования?
        Ship(shiptype type, const std::string& name, const std::string& captain_rank,
             const std::string& captain_name, double speed, int durability, double cost);

        // **Методы для любого судна**
        [[nodiscard]] shiptype getShipType() const override; // Получить тип корабля
        void setShipType(shiptype type) override;  // Установить тип корабля
        [[nodiscard]] std::string getName() const override; // Получить название судна
        void setName(const std::string &name) override; // Установить название судна
        [[nodiscard]] std::string getCaptainRank() const override; // Получить звание капитана
        [[nodiscard]] double getCost() const override;
        void setCost(double cost) override; // Установить стоимость для корабля
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
        void move() override; // Переместить корабль в точку назначения
        void receiveDamage(int damage) override; // Получить урон
        [[nodiscard]] double calculateTotalCost() const override;
        void setDestination(const ship::coordinate &new_destination) override;
    };

} // namespace acg

#endif // SHIP_H


#pragma clang diagnostic pop