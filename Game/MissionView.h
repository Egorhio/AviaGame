#pragma once

#include "ShipView.h"

namespace acg {

    class MissionView {
    public:
        static void showMainMenu(Mission* mission) ;
        static bool showNewGameMenu(Mission* mission) ;
//        void showShipManagement() const;
//        void showAircraftManagement() const;
//        void showCombatSimulation() const;
//        void showMissionStatus() const;
//        void updateMissionInfo() const;  // Новый метод
        static void showAdditionalSettings(Mission* mission) ;
    };

} // acg

