#include "MissionView.h"

namespace acg {

    void animateExitText() {
        std::string text = "Выход из игры...";
        for (size_t i = 0; i <= text.length(); ++i) {
            std::cout << "\r" << text.substr(0, i);
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::cout << std::endl;
    }

    void animateText() {
        // В ShipView.cpp
        std::cout << "\033[1;36m"; // Бирюзовый цвет
        std::cout << "    ███╗   ███╗ ██████╗ ██████╗ ███████╗██╗  ██╗ ██████╗ ██╗  ██  \n";
        std::cout << "    ████╗ ████║██╔═══██╗██╔══██╗██╔════╝██║ ██╔╝██╔═══██╗╚██╗██  \n";
        std::cout << "    ██╔████╔██║██║   ██║██████╔╝███████╗█████╔╝ ██║   ██║ ╚██╗ \n";
        std::cout << "    ██║╚██╔╝██║██║   ██║██╔══██╗╚════██║██╔═██╗ ██║   ██║  ██║ \n";
        std::cout << "    ██║ ╚═╝ ██║╚██████╔╝██║  ██║███████║██║  ██╗╚██████╔╝  ██║ \n";

        std::cout << "\033[1;35m"; // Пурпурный цвет
        std::cout << "\n    ██████╗  ██████╗ ██╗ ██  \n";
        std::cout << "    ██╔══██╗██╔═══██╗╚██╗ ██ \n";
        std::cout << "    ██████╔╝██║   ██║ ╚██╗ \n";
        std::cout << "    ██╔══██╗██║   ██║  ██║ \n";
        std::cout << "    ██████╔╝╚██████╔╝  ██║ \n\n";

        std::cout << "\033[0m"; // Сброс цвета
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void MissionView::showMainMenu(Mission* mission) {
        // Очистка экрана
        std::cout << "\033[2J\033[1;1H";
        static bool first_time = true;
        // ASCII-арт заголовка
        if (first_time) {
            std::string welcomeArt =
                    "██╗    ██╗███████╗██╗     ██████╗ ██████╗ ███╗   ███╗███████╗\n"
                    "██║    ██║██╔════╝██║     ██╔══  ╗██   ██╗████╗ ████║██╔════╝\n"
                    "██║ █╗ ██║█████╗  ██║     ██║    ║██   ██╔██╔████╔██║█████╗  \n"
                    "██║███╗██║██╔══╝  ██║     ██║    ║██   ██╗██║╚██╔╝██║██╔══╝  \n"
                    "╚███╔███╔╝███████╗███████╗██████╔╝█ ██ ██ ██║ ╚═╝ ██║███████╗\n"
                    " ╚══╝╚══╝ ╚══════╝╚══════╝╚═════╝ ╚══════╝╚═╝     ╚═╝╚══════╝\n";

            first_time = false;
            // Уменьшенная задержка
            for (char c: welcomeArt) {
                std::cout << c << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::cout << "\n\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            animateText();
        }

        int choice;
        std::string file_name;
        bool result;
        do {
            // Меню
            std::cout << "     ГЛАВНОЕ МЕНЮ" << std::endl;
            std::cout << "==========================================\n";
            std::cout << "1. Новая игра\n";
            std::cout << "2. Загрузить игру\n";
            std::cout << "3. Выйти\n";
            std::cout << "==========================================\n";
            std::cout << "\nВыберите действие: ";
            choice = getNumber<int>(1, 3);

            switch(choice) {
                case 1:
                    result = showNewGameMenu(mission);
                    if (result) return;
                    break;
                case 2:
                    std::cout << "Введите название файла: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    file_name = readline(std::cin);
                    mission->loadState(file_name);
                    return;
                case 3:
                    animateExitText();
                    return;
                default:
                    std::cout << "Неверный выбор\n";
            }
        } while (true);
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
    bool MissionView::showNewGameMenu(Mission* mission) {
        std::string commander_name, captain_rank;
        int choice, max_ships = 0, budget = 0;

        do {
            std::cout << "\n=== Новая игра ===\n";
            std::cout << "1. Вернуться в главное меню\n";
            std::cout << "2. Указать информацию о командире\n";
            std::cout << "3. Максимальное кол-во кораблей в каждой команде (до 30)\n";
            std::cout << "4. Бюджет на миссию для команды\n";
            std::cout << "5. Дополнительно... (указать координаты баз и необходимый ущерб для победы)\n";
            std::cout << "0. Продолжить\n";
            std::cout << "\nВыберите действие: ";
            choice = getNumber<int>();

            switch(choice) {
                case 5:
                    showAdditionalSettings(mission);
                    break;
                case 4:
                    std::cout << "Введите бюджет на миссию: ";
                    budget = getNumber<int>(1);
                    mission->setBudget(budget);
                    break;
                case 3:
                    std::cout << "Введите максимальное число кораблей: ";
                    max_ships = getNumber<int>(1, 30);
                    mission->setMaxShips(max_ships);
                    break;
                case 2:
                    std::cout << "Введите имя командира: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    commander_name = readline(std::cin);
                    std::cout << "Введите звание: ";
                    captain_rank = readline(std::cin);
                    std::cout << commander_name << std::endl << captain_rank << std::endl;
                    // Валидация введенных данных
                    if(commander_name.empty() || captain_rank.empty()) {
                        std::cout << "Ошибка: поля не могут быть пустыми\n";
                        continue;
                    }
                    mission->setCaptainRank(captain_rank);
                    mission->setCommander(commander_name);
                    break;
                case 1:
                    return false;
                case 0:
                    if(mission->getCommander().empty() || mission->getCaptainRank().empty()) {
                        std::cout << "Необходимо заполнить информацию о командире\n";
                        continue;
                    }
                    if(mission->getMaxShips() == 0) {
                        std::cout << "Необходимо указать максимальное число кораблей\n";
                        continue;
                    }
                    if(mission->getBudget() == 0) {
                        std::cout << "Необходимо указать бюджет на миссию\n";
                        continue;
                    }
                    return true;
                default:
                    std::cout << "Неверный выбор\n";
            }
        } while (choice != 0 || commander_name.empty() || captain_rank.empty());
    }
#pragma clang diagnostic pop

    void MissionView::showAdditionalSettings(Mission* mission) {
        int choice;
        do {
            std::cout << "\n=== Дополнительные настройки ===\n";
            std::cout << "1. Установить координаты базы A\n";
            std::cout << "2. Установить координаты базы B\n";
            std::cout << "3. Установить размер базы A\n";
            std::cout << "4. Установить размер базы B\n";
            std::cout << "5. Установить необходимый урон для победы\n";
            std::cout << "0. Назад\n";
            std::cout << "\nВыберите действие: ";

            choice = getNumber<int>(0, 5);

            switch(choice) {
                case 1: {
                    std::cout << "Введите координаты X и Y для базы A: ";
                    auto x = getNumber<double>(0);
                    auto y = getNumber<double>(0);
                    mission->setBaseACoordinates({x, y});
                    break;
                }
                case 2: {
                    std::cout << "Введите координаты X и Y для базы B: ";
                    auto x = getNumber<double>(0);
                    auto y = getNumber<double>(0);
                    mission->setBaseBCoordinates({x, y});
                    break;
                }
                case 3: {
                    std::cout << "Введите размер базы A: ";
                    auto size = getNumber<float>(1);
                    mission->setSizeBaseA(size);
                    break;
                }
                case 4: {
                    std::cout << "Введите размер базы B: ";
                    auto size = getNumber<float>(1);
                    mission->setSizeBaseB(size);
                    break;
                }
                case 5: {
                    std::cout << "Введите необходимый урон для победы: ";
                    auto damage = getNumber<double>(20);
                    mission->setNecessaryDamage(damage);
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "Неверный выбор\n";
                    break;
            }
        } while (true);
    }


} // acg