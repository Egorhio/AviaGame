#pragma once

#include "BattlefieldView.h"
#include "MissionView.h"
#include "../AI/ai.h"

namespace acg {

    class GameController {
        Mission* mission;
        MissionView missionView;
        BattlefieldView battlefieldView;
        AI *ai;

    public:
//        void gameLoop() {
//            while (!gameOver) {
//                handlePlayerTurn();
//                ai->makeMove();
//                view.updateShipPositions(mission);
//                view.displayFieldWithCoordinates();
//            }
//        }
    };
}