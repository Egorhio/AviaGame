#pragma once

#pragma once
#include "BattlefieldView.h"
#include "MissionView.h"
#include "../AI/ai.h"

namespace acg {
    class GameController {
        bool gameOver{false};
        Mission* mission;
        BattlefieldView view;
        AI ai;
    public:
        explicit GameController(Mission* mission);
        void gameProcess();
        void handlePlayerTurn();
        bool checkGameOver();
    };
}