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
        [[nodiscard]] shiptype getShipType() const; // Получить тип корабля
        void setShipType(shiptype type);  // Установить тип корабля
        [[nodiscard]] std::string getName() const; // Получить название судна
        void setName(const std::string &name); // Установить название судна
        [[nodiscard]] std::string getCaptainRank() const; // Получить звание капитана
        [[nodiscard]] double getCost() const;
        void setCost(double cost); // Установить стоимость для корабля
        void setCaptainRank(const std::string &rank); // Установить звание капитана
        [[nodiscard]] std::string getCaptainName() const; // Получить имя капитана
        void setCaptainName(const std::string &name); // Установить имя капитана
        [[nodiscard]] double getSpeed() const; // Получить текущую скорость
        void setSpeed(double speed); // Установить текущую скорость
        [[nodiscard]] int getDurability() const; // Получить живучесть судна
        void setDurability(int durability); // Установить живучесть судна

        // *Методы работы с координатами*
        [[nodiscard]] ship::coordinate getCurrentCoordinates() const; // Получить текущую точку
        void setCurrentCoordinates(const ship::coordinate& coordinates); // Установить текущую точку
        [[nodiscard]] std::optional<ship::coordinate> getDestinationCoordinates() const; // Получить точку назначения
        void setDestinationCoordinates(const ship::coordinate& coordinates); // Установить точку назначения

        // *Дополнительные базовые методы*
        void move() override; // Переместить корабль в точку назначения
        void receiveDamage(int damage); // Получить урон
        [[nodiscard]] double calculateTotalCost() const override;
        void setDestination(const ship::coordinate &new_destination) override;

    };

    //** доп метод
    double calculateDistance(const ship::coordinate &target_coordinates, const ship::coordinate &current_pos);

} // namespace acg

#endif // SHIP_H