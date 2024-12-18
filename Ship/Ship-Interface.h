#pragma once

#include "Armament.h"
#include "Aircraft.h"
#include <vector>
#include <map>
#include <valarray>
#include <optional>

namespace acg {

    namespace ship {
        struct AmmoInfo { // Информация о боеприпасах
            int quantity = 0;         // Текущее количество
            double size = 0;          // Размер единицы боеприпаса
            double cost = 0;          // Стоимость единицы
        };

        using armvector = std::vector<Armament>;
        using ammomap = std::map<std::string, AmmoInfo>;
        using coordinate = std::pair<double, double>;
        using airvector = std::vector<std::pair<Aircraft, coordinate>>;

    } // namespace ship

    class IShip {
    public:
        virtual ~IShip() = default;
        enum class shiptype {
            CRUISER,
            AIRCRAFTCARRIER,
            AVIATORCRUISER
        };

        // **Методы для любого судна**
        // **Это геттеры и сеттеры, для которых не нужно переопределение**

        // *Дополнительные базовые методы*
        [[nodiscard]] virtual double calculateTotalCost() const = 0;
        virtual void setDestination(const ship::coordinate &new_destination) = 0;
        virtual void move() = 0; // Переместить корабль в точку назначения

    };
} // namespace acg
