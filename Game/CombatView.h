#pragma once

#include "Mission.h"

namespace acg {

    class CombatView {
    public:
        void displayAirRaidMenu() const;  // Добавить const
        void showBattleResults(const ShipTable<Ship>& ships) const;  // Добавить параметр
        void displayTargetSelection(const ship::coordinate& coords) const;
        void showDamageReport(const Ship* ship) const;
        void showCombatStatus(const ShipTable<Ship>& ships) const;  // Новый метод
    };

} // acg
