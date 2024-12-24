/**
 * @file AviatorCruiser-Interface.h
 * @brief Интерфейс для всех авианесущих крейсеров в игре
 * @author Egor Volkov
 * @date 2024
 */

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
