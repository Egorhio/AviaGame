#include "main-game.h"

using namespace acg;
using namespace std;

int main() {

    auto* mission = new Mission;
    mission->setMaxShips(20);
    mission->setBudget(100000);

    auto * game = new GameController(mission);

    game->gameProcess();


    delete mission;
    delete game;
    return 0;
}