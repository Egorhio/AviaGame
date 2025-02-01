#pragma once

#include "BattlefieldView.h"
#include "../1/additional.h"

namespace acg {

    using Prog1::getNumber;
    using Prog1::readline;

    void showShipMenu(Mission* mission);
    MissionError showBuyShipMenu(Mission* mission);
    MissionError showBuyWeaponMenu(Mission* mission);
    MissionError showBuyAircraftMenu(Mission* mission);
    void showAllShipsInfo(Mission* mission);
    MissionError showTransferMenu(const ShipTable<Ship>& ships);

} // acg

