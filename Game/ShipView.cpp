#include "ShipView.h"

namespace acg {

    void displayAnimatedText(const std::string& text) {
        static bool first_time = true;
        std::cout << "\033[1;36m"; // Устанавливаем бирюзовый цвет и жирный шрифт
        for(char c : text) {
            std::cout << c << std::flush;
            if (first_time)
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Задержка для анимации
        }
        std::cout << "\033[0m\n"; // Сброс форматирования
        first_time = false;
    }


    void showShipMenu(Mission* mission) {
        if (!mission) return;
        while (true) {
            std::cout << "\n=================================\n";
            displayAnimatedText("    Покупка транспорта и оружия    ");
            std::cout << "=================================\n";
            std::cout << "1. Продать/Купить судно\n";
            std::cout << "2. Продать/Купить оружие для судна\n";
            std::cout << "3. Продать/Купить самолёты для судна\n";
            std::cout << "4. Имеющийся ассортимент всего сейчас\n";
            std::cout << "0. Начать игру\n";
            std::cout << "Бюджет: " << mission->getRemainingBudget() << "\n";
            std::cout << "Выберите действие: ";

            int choice = getNumber(0, 4);
            MissionError result = MissionError::INVALID_SHIP_TYPE;

            switch (choice) {
                case 4: {
                    if (!(mission->getShipGroupTable().getShipCount())) {
                        std::cout << "У вас нет кораблей\n";
                        continue;
                    }
                    showAllShipsInfo(mission);
                    break;
                }
                case 1: {
                    std::cout << "\n1. Купить судно\n2. Продать судно\nВыбор: ";
                    int subChoice = getNumber<int>(1, 2);
                    if (subChoice == 1) {
                        result = showBuyShipMenu(mission);
                    } else if (subChoice == 2) {
                        std::cout << "Введите позывной корабля: ";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::string callSign = readline(std::cin);
                        result = mission->sellShip(callSign);
                    }
                    break;
                }
                case 2: {
                    std::cout << "Введите позывной корабля: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::string callSign = readline(std::cin);
                    Ship* ship = mission->getShipGroupTable().getShip(callSign);
                    if (ship && (ship->getShipType() == Ship::shiptype::CRUISER ||
                                 ship->getShipType() == Ship::shiptype::AVIATORCRUISER)) {
                        std::cout << "\n1. Купить вооружение\n2. Продать вооружение\nВыбор: ";
                        int subChoice = getNumber<int>(1, 2);
                        if (subChoice == 1) {
                            result = showBuyWeaponMenu(mission);
                        } else if (subChoice == 2) {
                            // Показываем текущее вооружение корабля
                            BattlefieldView::displayShipInfo(ship, callSign, true);
                            std::cout << "\nВыберите оружие для продажи (1-" <<
                                      (ship->getShipType() == Ship::shiptype::CRUISER ?
                                       dynamic_cast<ICruiser*>(ship)->getArmament().size() :
                                       dynamic_cast<IAviatorCruiser*>(ship)->getArmament().size()) << "): ";
                            int weaponChoice = getNumber<int>(1, 10);

                            auto weapons = ship->getShipType() == Ship::shiptype::CRUISER ?
                                           dynamic_cast<ICruiser*>(ship)->getArmament() :
                                           dynamic_cast<IAviatorCruiser*>(ship)->getArmament();

                            if (weaponChoice <= weapons.size()) {
                                result = mission->sellWeaponFromShip(callSign, &weapons[weaponChoice-1]);
                            }
                        }
                    } else {
                        std::cout << "Корабль не найден или не поддерживает вооружение\n";
                        continue;
                    }
                    break;
                }
                case 3: {
                    std::string callSign;
                    std::cout << "Введите позывной корабля: ";
                    std::cin >> callSign;
                    Ship* ship = mission->getShipGroupTable().getShip(callSign);
                    if (ship && (ship->getShipType() == Ship::shiptype::AIRCRAFTCARRIER ||
                                 ship->getShipType() == Ship::shiptype::AVIATORCRUISER)) {
                        std::cout << "\n1. Купить самолёт\n2. Продать самолёт\nВыбор: ";
                        int subChoice = getNumber<int>(1, 2);
                        if (subChoice == 1) {
                            result = showBuyAircraftMenu(mission);
                        } else if (subChoice == 2) {
                            // Показываем текущие самолёты корабля
                            BattlefieldView::displayShipInfo(ship, callSign, true);
                            std::cout << "\nВыберите самолёт для продажи (1-" <<
                                      (ship->getShipType() == Ship::shiptype::AIRCRAFTCARRIER ?
                                       dynamic_cast<IAircraftCarrier*>(ship)->getAircrafts().size() :
                                       dynamic_cast<IAviatorCruiser*>(ship)->getAircrafts().size()) << "): ";
                            int aircraftChoice = getNumber<int>(1, 10);

                            auto aircraft = ship->getShipType() == Ship::shiptype::AIRCRAFTCARRIER ?
                                            dynamic_cast<IAircraftCarrier*>(ship)->getAircrafts() :
                                            dynamic_cast<IAviatorCruiser*>(ship)->getAircrafts();

                            if (aircraftChoice <= aircraft.size()) {
                                result = mission->sellPlaneFromShip(callSign, &aircraft[aircraftChoice-1].first);
                            }
                        }
                    } else {
                        std::cout << "Корабль не найден или не поддерживает авиацию\n";
                        continue;
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "Неверный выбор\n";
                    continue;
            }

            // Вывод результата операции
            switch (result) {
                case MissionError::SUCCESS:
                    std::cout << "Операция успешно выполнена\n";
                    break;
                case MissionError::INSUFFICIENT_FUNDS:
                    std::cout << "Недостаточно средств\n";
                    break;
                case MissionError::SHIP_NOT_FOUND:
                    std::cout << "Корабль не найден\n";
                    break;
                case MissionError::STORAGE_FULL:
                    std::cout << "Хранилище заполнено\n";
                    break;
                case MissionError::EMPTY_CALLSIGN:
                    std::cout << "Пустой позывной\n";
                    break;
                default:
                    std::cout << "Неизвестная ошибка\n";
            }
        }
    }

    MissionError showBuyShipMenu(Mission* mission) {
        std::vector<Ship*> ships;
        std::vector<std::string> callsigns = {"SHIP1", "SHIP2", "SHIP3", "SHIP4", "SHIP5"};

        // Генерируем 5 случайных кораблей
        for (int i = 0; i < 5; i++) {
            std::random_device rd;
            std::mt19937 gen(rd());

            // Случайный тип корабля
            std::uniform_int_distribution<> type_dist(0, 2);
            auto type = static_cast<Ship::shiptype>(type_dist(gen));

            // Случайные характеристики
            std::uniform_real_distribution<> speed_dist(20.0, 40.0);
            std::uniform_int_distribution<> durability_dist(50, 100);
            std::uniform_real_distribution<> cost_dist(500.0, 2000.0);

            Ship* ship = nullptr;
            switch(type) {
                case Ship::shiptype::AIRCRAFTCARRIER:
                    ship = new AircraftCarrier(type, "Carrier", "Captain", "John",
                                               speed_dist(gen), durability_dist(gen), cost_dist(gen));
                    break;
                case Ship::shiptype::CRUISER:
                    ship = new Cruiser(type, "Cruiser", "Captain", "Jack",
                                       speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000);
                    break;
                case Ship::shiptype::AVIATORCRUISER:
                    ship = new AviatorCruiser(type, "Aviator", "Captain", "James",
                                              speed_dist(gen), durability_dist(gen), cost_dist(gen), 5, 1000, 5);
                    break;
            }
            ships.push_back(ship);
        }

        // Показываем корабли пользователю
        std::cout << "Доступные корабли для покупки:\n";
        for (int i = 0; i < 5; i++) {
            std::cout << "\n[" << i + 1 << "] ";
            BattlefieldView::displayShipInfo(ships[i], callsigns[i], true);
        }

        // Выбор корабля пользователем
        int choice;
        do {
            std::cout << "\nВыберите корабль (1-5): ";
            std::cin >> choice;
        } while (choice < 1 || choice > 5);

        // Генерация случайных координат (кроме 0,0 и 19,19)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> coord_dist(1, 18); // От 1 до 18, исключая 0 и 19

        // Генерируем координаты, пока не получим уникальные
        bool valid_coords = false;
        ship::coordinate coords;
        while (!valid_coords) {
            coords.first = coord_dist(gen);
            coords.second = coord_dist(gen);

            // Проверяем, что координаты не совпадают с базами (0,0) и (19,19)
            if ((coords.first != 0 || coords.second != 0) &&
                (coords.first != 19 || coords.second != 19)) {
                valid_coords = true;
            }
        }

        // Устанавливаем сгенерированные координаты для корабля
        ships[choice-1]->setCurrentCoordinates(coords);

        // Покупка выбранного корабля
        MissionError result = mission->buyShip(callsigns[choice-1], ships[choice-1]);

        // Очистка неиспользованных кораблей
        for (int i = 0; i < 5; i++) {
            if (i != choice-1) delete ships[i];
        }
        return result;
    }

    MissionError showBuyWeaponMenu(Mission* mission) {
        // Проверяем наличие кораблей подходящего типа
        bool hasValidShip = false;
        auto iter = mission->getShipGroupTable().getIterator();
        while (iter.hasNext()) {
            auto [_, ship] = iter.get();
            if (ship->getShipType() == Ship::shiptype::CRUISER ||
                ship->getShipType() == Ship::shiptype::AVIATORCRUISER) {
                hasValidShip = true;
                break;
            }
            iter.next();
        }

        if (!hasValidShip) {
            std::cout << "Нет кораблей, способных нести оружие!\n";
            return MissionError::INVALID_SHIP_TYPE;
        }

        // Генерируем оружие с разными характеристиками
        ship::armvector weapons;

        // Легкое оружие
        weapons.emplace_back("Light Cannon",
                             Armament::ArmamentType::LIGHT,
                             50, 800.0, 45.0, 100, 2.0, 600.0);

        weapons.emplace_back("Heavy Machine Gun",
                             Armament::ArmamentType::LIGHT,
                             35, 600.0, 60.0, 200, 3.0, 450.0);

        // Тяжелое оружие
        weapons.emplace_back("Heavy Cannon",
                             Armament::ArmamentType::HEAVY,
                             120, 1200.0, 30.0, 50, 4.0, 1000.0);

        weapons.emplace_back("Missile Launcher",
                             Armament::ArmamentType::HEAVY,
                             200, 2000.0, 15.0, 20, 6.0, 1500.0);

        weapons.emplace_back("Torpedo System",
                             Armament::ArmamentType::HEAVY,
                             180, 1500.0, 20.0, 30, 5.0, 1200.0);

        // Показываем доступное оружие
        std::cout << "\nДоступное оружие:\n";
        int i = 1;
        for (const auto & weapon : weapons) {
            std::cout << "[" << i++ << "]" << std::endl;
            BattlefieldView::displayWeaponInfo(weapon);
        }

        // Пользователь выбирает оружие
        int choice;
        std::cout << "Выберите оружие (1-5): ";
        std::cin >> choice;
        choice--;
        if (choice < 0 || choice >= weapons.size()) {
            return MissionError::WEAPON_NOT_FOUND;
        }

        // Выбор корабля для установки оружия
        std::cout << "Введите позывной корабля: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string callSign = readline(std::cin);

        return mission->buyWeaponForShip(callSign, &weapons[choice]);
    }

    MissionError showBuyAircraftMenu(Mission* mission) {
        std::cout << "\nВведите позывной корабля: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string callSign = readline(std::cin);

        Ship* ship = mission->getShipGroupTable().getShip(callSign);
        if (!ship) return MissionError::SHIP_NOT_FOUND;

        // Проверяем, что корабль может нести авиацию
        if (ship->getShipType() != Ship::shiptype::AIRCRAFTCARRIER &&
            ship->getShipType() != Ship::shiptype::AVIATORCRUISER) {
            return MissionError::INVALID_SHIP_TYPE;
        }

        std::cout << "\n1. Истребитель (500)\n2. Бомбардировщик (700)\nВыбор: ";
        int choice = getNumber<int>(1, 2);

        Aircraft* aircraft;
        if (choice == 1) {
            aircraft = new Aircraft(Aircraft::AircraftType::FIGHTER,
                                    100, true, 100, 50.0, 10.0,
                                    1000.0, 20.0, 500.0, 1000.0);
        } else {
            aircraft = new Aircraft(Aircraft::AircraftType::ATTACK,
                                    150, true, 80, 40.0, 15.0,
                                    1200.0, 25.0, 700.0, 1500.0);
        }

        MissionError result = mission->buyPlaneForShip(callSign, aircraft);
        if (result != MissionError::SUCCESS) {
            delete aircraft;
        }

        return result;
    }

    void showAllShipsInfo(Mission* mission) {
        if (!mission) return;

        auto iter = mission->getShipGroupTable().getIterator();
        std::cout << "\n=== Информация о всех кораблях ===\n";
        std::cout << mission->getShipGroupTable().getShipCount() << std::endl;

        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            BattlefieldView::displayShipInfo(ship, callSign, true);
            std::cout << "\n---------------------------\n";
            iter.next();
        }
    }
}