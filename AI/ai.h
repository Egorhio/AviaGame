#pragma once

#include "Mission.h"

namespace acg {

    class AI {
    private:
        Mission* mission;
        std::string aiTeamCallsign;

    public:
        explicit AI(Mission* mission, std::string  teamCallsign)
        : mission(mission), aiTeamCallsign(std::move(teamCallsign)) {}

        void makeMove() {
            // Основная логика принятия решений
            analyzeGameState();
            planStrategy();
            executeActions();
        }

        void analyzeGameState();

        void planStrategy();

        void executeActions();

        void makeDecision();
    };
}


