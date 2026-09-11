#include "main-game.h"

#include <algorithm>
#include <iostream>

namespace acg {

    GameController::GameController(Mission* mission) {
        this->mission = mission;
        view.initField();
    }

    void GameController::gameProcess() {
        try {
            if (!MissionView::showMainMenu(mission)) {
                return; // Игрок выбрал выход из главного меню
            }
            showShipMenu(mission);
            AI::generateRandomAIFleet(mission, AI_TEAM_PREFIX);

            view.updateShipPositions(*mission);
            displayAnimatedTextGame("ИГРА НАЧАЛАСЬ");
            view.displayFieldWithCoordinates();

            while (!gameOver) {
                try {
                    handlePlayerTurn();
                    mission->removeDestroyedShips();
                    view.updateShipPositions(*mission);
                    if (!gameOver) gameOver = checkGameOver();

                    if (!gameOver) {
                        try {
                            std::cout << "Ход ИИ..." << std::endl;
                            std::this_thread::sleep_for(std::chrono::milliseconds(300));
                            ai.makeMove(mission);
                            mission->removeDestroyedShips();
                            view.updateShipPositions(*mission);
                            view.displayFieldWithCoordinates();
                            gameOver = checkGameOver();
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Ошибка в ходе ИИ: " << e.what() << std::endl;
                        }
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Ошибка в ходе игрока: " << e.what() << std::endl;
                    // Поток ввода закрыт/сломан — продолжать игру нельзя, иначе
                    // бесконечный цикл на ошибке чтения.
                    if (!std::cin || std::cin.eof()) {
                        std::cerr << "Ввод недоступен, выход из игры." << std::endl;
                        gameOver = true;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Критическая ошибка в игровом процессе: " << e.what() << std::endl;
        }
    }

    void GameController::handlePlayerTurn() {
        std::cout << "\nВаш ход:\n";
        std::cout << "1. Переместить корабль\n";
        std::cout << "2. Атаковать\n";
        std::cout << "3. Показать поле\n";
        std::cout << "4. Остановка и сохранение на диск\n";
        std::cout << "0. Сдаться\n";

        int choice = Prog1::getNumber<int>(0, 4);
        std::string name;

        switch(choice) {
            case 1:
                view.handleShipMovement(mission);
                break;
            case 2:
                view.handleAttack(mission);
                break;
            case 3:
                view.displayFieldWithCoordinates();
                handlePlayerTurn();
                break;
            case 4:
                std::cout << "Имя файла для сохранение (.json): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                name = readline(std::cin);
                mission->saveState(name);
                gameOver = true;
                break;
            case 0:
                gameOver = true;
                break;
            default:
                break;
        }
    }

    bool GameController::checkGameOver() {
        const auto baseB = mission->getBaseBCoordinates();
        const double reach = std::max(1.0f, mission->getSizeBaseB());

        bool playerHasShips = false;
        bool aiHasShips = false;

        for (auto iter = mission->getShipGroupTable().getIterator(); iter.hasNext(); iter.next()) {
            auto [callSign, ship] = iter.get();
            const bool isAI = callSign.find(AI_TEAM_PREFIX) != std::string::npos;
            if (isAI) {
                aiHasShips = true;
            } else {
                playerHasShips = true;
                if (calculateDistance(ship->getCurrentCoordinates(), baseB) <= reach) {
                    std::cout << "Корабль " << callSign << " достиг базы B. Победа!\n";
                    return true;
                }
            }
        }

        // Победа по нанесённому ущербу противнику (цель первой флотилии)
        if (mission->getNecessaryDamage() > 0 &&
            mission->getDamagePerGroup() >= mission->getNecessaryDamage()) {
            std::cout << "Достигнут необходимый ущерб противнику ("
                      << mission->getDamagePerGroup() << " / "
                      << mission->getNecessaryDamage() << "). Победа!\n";
            return true;
        }

        if (!playerHasShips) std::cout << "У вас не осталось кораблей. Поражение.\n";
        else if (!aiHasShips) std::cout << "Флот противника уничтожен. Победа!\n";
        return !playerHasShips || !aiHasShips;
    }

}