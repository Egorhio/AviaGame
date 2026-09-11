#pragma once

#include "Mission.h"
#include "../common/additional.h"
#include <random>

namespace acg {

    class BattlefieldView {
    private:
        int FIELD_SIZE;
        std::vector<std::vector<char>> field;
    public:
        BattlefieldView();  // Добавить конструктор 1
        explicit BattlefieldView(int);  // Добавить конструктор 2
        void initField();
        void updateShipPositions(Mission& mission);
        void displayField() const;
        void displayFieldWithCoordinates() const;

        void generateBasePositions(Mission &mission);
        void updateShipPositions(Mission &mission, ship::coordinate baseA, ship::coordinate baseB);
        static void displayWeaponInfo(const Armament &weapon);
        static void displayAircraftInfo(const Aircraft &aircraft);
        static void displayShipsInfo(const ship::coordinate &coords, const Mission &mission, bool fullinfo);
        static void displayShipsInfo(const std::string &callSign, const Mission &mission, bool fullinfo);
        static void displayShipInfo(const Ship* ship, const std::string& callSign, bool fullinfo);

        void handleShipMovement(Mission *mission);

        void handleAttack(Mission *mission);
    };

} // acg

