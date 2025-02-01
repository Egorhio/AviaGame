#include "ai.h"

namespace acg {

    void AI::analyzeGameState() {
        if (!mission) return;

        // Анализ состояния своих кораблей
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                // Проверка состояния корабля
                if (ship->getDurability() < 30) {
                    // Отступление к ближайшей базе
                    auto baseA = mission->getBaseACoordinates();
                    auto baseB = mission->getBaseBCoordinates();
                    auto currentPos = ship->getCurrentCoordinates();

                    // Выбор ближайшей базы
                    double distToA = calculateDistance(currentPos, baseA);
                    double distToB = calculateDistance(currentPos, baseB);
                    ship->setDestinationCoordinates(distToA < distToB ? baseA : baseB);
                }
            }
            iter.next();
        }
    }

    void AI::planStrategy() {
        if (!mission) return;

        // Стратегия атаки для авианосцев
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                if (ship->getShipType() == Ship::shiptype::AIRCRAFTCARRIER ||
                    ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {

                    // Поиск целей для атаки
                    auto enemyIter = mission->getShipGroupTable().getIterator();
                    while (enemyIter.hasNext()) {
                        auto [enemyCallSign, enemyShip] = enemyIter.get();
                        if (enemyCallSign.find(aiTeamCallsign) == std::string::npos) {
                            auto enemyPos = enemyShip->getCurrentCoordinates();
                            // Планирование воздушного налета
                            mission->simulateAirRaid(callSign, enemyPos);
                            break;
                        }
                        enemyIter.next();
                    }
                }
            }
            iter.next();
        }
    }

    void AI::executeActions() {
        if (!mission) return;

        // Выполнение запланированных действий
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find(aiTeamCallsign) != std::string::npos) {
                // Движение к цели
                auto dest = ship->getDestinationCoordinates();
                auto current = ship->getCurrentCoordinates();
                if (dest != current) {
                    ship->move();
                }

                // Атака ближайших целей для крейсеров
                if (ship->getShipType() == Ship::shiptype::CRUISER ||
                    ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                    auto* cruiser = dynamic_cast<ICruiser*>(ship);
                    if (cruiser) {
                        auto enemyIter = mission->getShipGroupTable().getIterator();
                        while (enemyIter.hasNext()) {
                            auto [_, enemyShip] = enemyIter.get();
                            cruiser->fireAtShip(enemyShip->getCurrentCoordinates());
                            break;
                        }
                        enemyIter.next();
                    }
                }
            }
            iter.next();
        }
    }
}