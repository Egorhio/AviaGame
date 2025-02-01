#pragma once

#include "ShipView.h"

namespace acg {

    class MissionView {
    public:
        static void showMainMenu(Mission* mission) ;
        static bool showNewGameMenu(Mission* mission) ;
        static void showAdditionalSettings(Mission* mission) ;
    };

    void displayAnimatedTextGame(const std::string& text);


} // acg

