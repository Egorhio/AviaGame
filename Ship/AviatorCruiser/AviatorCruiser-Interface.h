#pragma once

#include "../AircraftCarrier/AircraftCarrier-Interface.h"
#include "../Cruiser/Cruiser-Interface.h"

namespace acg {

    class IAviatorCruiser : public IAircraftCarrier, public ICruiser {
    public:

        ~IAviatorCruiser() override = default;

        void interceptorAttack(const ship::airvector& enemy_aircraft) override = 0;

        void fireAtAircraft(const ship::airvector& enemy_aircraft) override = 0;
    };


} // namespace acg
