#pragma once

#include "Mission.h"
#include <random>

namespace acg {

    class AI {
    private:
        std::string aiTeamCallsign;

    public:
        AI() = default;

        explicit AI(std::string  teamCallsign)
        :  aiTeamCallsign(std::move(teamCallsign)) {}

        void makeMove(Mission* mission) {
            // Основная логика принятия решений
            analyzeGameState(mission);
            planStrategy(mission);
            executeActions(mission);
        }

        void analyzeGameState(Mission* mission);
        void planStrategy(Mission* mission);
        void executeActions(Mission* mission);

        static MissionError generateRandomAIFleet(Mission* mission, const std::string& aiTeamPrefix);
        static void generateRandomAircraft(Mission* mission, const std::string& callSign);
        static void generateRandomWeapons(Mission* mission, const std::string& callSign);
    };
}


