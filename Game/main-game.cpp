#include "main-game.h"

namespace acg {

    GameController::GameController(Mission* mission) {
        this->mission = mission;
        view.initField();
    }

    void GameController::gameProcess() {
        try {
            MissionView::showMainMenu(mission);
            showShipMenu(mission);
            AI::generateRandomAIFleet(mission, "AI");

            view.updateShipPositions(*mission);
            displayAnimatedTextGame("ИГРА НАЧАЛАСЬ");
            view.displayFieldWithCoordinates();

            while (!gameOver) {
                try {
                    handlePlayerTurn();
                    view.updateShipPositions(*mission);

                    if (!gameOver) {
                        try {
                            std::cout << "Ход ИИ..." << std::endl;
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                            ai.makeMove(mission);
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
        auto baseB = mission->getBaseBCoordinates();
        auto iter = mission->getShipGroupTable().getIterator();

        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find("AI") == std::string::npos) {
                auto pos = ship->getCurrentCoordinates();
                if (pos == baseB) {
                    return true;
                }
            }
            iter.next();
        }

        bool playerHasShips = false;
        bool aiHasShips = false;
        iter = mission->getShipGroupTable().getIterator();

        while(iter.hasNext()) {
            auto [callSign, _] = iter.get();
            if (callSign.find("AI") != std::string::npos) {
                aiHasShips = true;
            } else {
                playerHasShips = true;
            }
            iter.next();
        }

        return !playerHasShips || !aiHasShips;
    }

}