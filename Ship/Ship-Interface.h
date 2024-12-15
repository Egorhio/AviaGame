#ifndef SHIP_INTERFACE_H
#define SHIP_INTERFACE_H

#include "Armament.h"
#include "Aircraft.h"
#include <vector>
#include <map>
#include <valarray>

namespace acg {

    namespace ship {
        struct AmmoInfo { // Информация о боеприпасах
            int quantity = 0;         // Текущее количество
            double size = 0;          // Размер единицы боеприпаса
            double cost = 0;          // Стоимость единицы
        };

        using armvector = std::vector<Armament>;
        using ammomap = std::map<std::string, AmmoInfo>;
        using airvector = std::vector<Aircraft>;
        using coordinate = std::pair<double, double>;
    } // namespace ship

    class IShip {
    public:
        virtual ~IShip() = default;
        enum class shiptype { CRUISER, AIRCRAFTCARRIER, AVIATORCRUISER };

        // **Методы для любого судна**
        [[nodiscard]] virtual shiptype getShipType() const = 0; // Получить тип корабля
        virtual void setShipType(shiptype type) = 0;  // Установить тип корабля
        [[nodiscard]] virtual std::string getName() const = 0; // Получить название судна
        virtual void setName(const std::string &name) = 0; // Установить название судна
        [[nodiscard]] virtual std::string getCaptainRank() const = 0; // Получить звание капитана
        virtual void setCaptainRank(const std::string &rank) = 0; // Установить звание капитана
        [[nodiscard]] virtual std::string getCaptainName() const = 0; // Получить имя капитана
        virtual void setCaptainName(const std::string &name) = 0; // Установить имя капитана
        [[nodiscard]] virtual double getSpeed() const = 0; // Получить текущую скорость
        virtual void setSpeed(double speed) = 0; // Установить текущую скорость
        [[nodiscard]] virtual int getDurability() const = 0; // Получить живучесть судна
        virtual void setDurability(int durability) = 0; // Установить живучесть судна

        // *Методы работы с координатами*
        [[nodiscard]] virtual ship::coordinate getCurrentCoordinates() const = 0; // Получить текущую точку
        virtual void setCurrentCoordinates(const ship::coordinate& coordinates) = 0; // Установить текущую точку
        [[nodiscard]] virtual ship::coordinate getDestinationCoordinates() const = 0; // Получить точку назначения
        virtual void setDestinationCoordinates(const ship::coordinate& coordinates) = 0; // Установить точку назначения

        // *Дополнительные базовые методы*
        [[nodiscard]] virtual double getCost() const = 0; // Вернуть стоимость корабля (суммарную)
        virtual void setCost(double cost) = 0; // Установить стоимость для корабля
        [[nodiscard]] virtual double calculateTotalCost() const = 0;
        virtual void setDestination(const ship::coordinate &new_destination) = 0;
        virtual void move() = 0; // Переместить корабль в точку назначения
        virtual void receiveDamage(int damage) = 0; // Получить урон

    };
} // namespace acg

#endif //SHIP_INTERFACE_H
