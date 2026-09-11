#pragma once

#include "BattlefieldView.h"
#include "MissionView.h"
#include "../AI/ai.h"

namespace acg {
    /// Префикс позывных кораблей, которыми управляет ИИ противника
    inline constexpr const char* AI_TEAM_PREFIX = "AI";

    class GameController {
        bool gameOver{false};
        Mission* mission;
        BattlefieldView view;
        AI ai{AI_TEAM_PREFIX};
    public:
        explicit GameController(Mission* mission);
        void gameProcess();
        void handlePlayerTurn();
        bool checkGameOver();
    };
}