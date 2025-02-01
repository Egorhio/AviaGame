#include "main-game.h"

using namespace std;
using namespace acg;

int main() {
    cout <<  "Welcome to the game!" << endl;

    BattlefieldView newview(20);
    newview.initField();

    Mission mission("im", 10, 10000);
    auto* carrier = new AircraftCarrier(
            Ship::shiptype::AIRCRAFTCARRIER,
            "TestCarrier", "Captain", "John", 30.0, 100, 1000.0
    );

    auto* aviator = new AviatorCruiser(
            Ship::shiptype::AVIATORCRUISER,
            "TestAviator",
            "Captain",
            "John",
            20,
            50,
            10,
            20,
            30,
            2);

    auto aircraft = Aircraft(
            acg::Aircraft::AircraftType::FIGHTER, 100, true, 200.0, 50, 1000.0, 100.0, 100, 100, 4
    );

    ship::coordinate coordinates(8, 4);
    ship::coordinate coordinates2(4, 1);
    ship::coordinate coordinatesA(1, 1);
    ship::coordinate coordinatesB(7, 9);
    mission.setBaseACoordinates(coordinatesA);
    mission.setBaseBCoordinates(coordinatesB); // TODO Неактивированное оружие почему то
    mission.setSizeBaseA(5);  // TODO Убрать неизвестную ошибка в ShipView при вызове информации обо всех кораблях
    carrier->setCurrentCoordinates(coordinates); // TODO Отловить как можно больше ошибок
    aviator->setCurrentCoordinates(coordinates2); // TODO продумать логику таким образом, чтобы корабли были от базы недалеко

    mission.buyShip("im", carrier);
    carrier->setMaxAircraftCapacity(10);
    mission.buyPlaneForShip("im", &aircraft);
    mission.buyShip("im2", aviator);
    newview.updateShipPositions(mission);
    //newview.displayFieldWithCoordinates();
    //BattlefieldView::displayShipInfo("im", mission, false);
    mission.saveState("file.json");

    //delete aircraft;

    return 0;
}
