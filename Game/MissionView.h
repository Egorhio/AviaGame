#pragma once

#include "ShipView.h"

namespace acg {

    class MissionView {
    public:
        /// @return true — начать игру (новая или загруженная); false — игрок выбрал выход
        static bool showMainMenu(Mission* mission) ;
        /// @return true — параметры заданы, начать игру; false — вернуться в главное меню
        static bool showNewGameMenu(Mission* mission) ;
        static void showAdditionalSettings(Mission* mission) ;
    };

    void displayAnimatedTextGame(const std::string& text);


} // acg

