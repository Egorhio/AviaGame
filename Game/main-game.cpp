#include "main-game.h"

using namespace acg;
using namespace std;

int main() {

    auto* mission = new Mission;
    mission->setMaxShips(20);
    mission->setBudget(100000);

    //MissionView::showMainMenu(mission);
    showShipMenu(mission);
//    BattlefieldView view(20);
//    view.initField();
//    view.updateShipPositions(*mission);
//    view.displayFieldWithCoordinates();


    delete mission;
    return 0;
}