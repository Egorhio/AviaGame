#include "BattlefieldView.h"

namespace acg {

    BattlefieldView::BattlefieldView() {
        FIELD_SIZE = 20; // Значение по умолчанию
        field.resize(FIELD_SIZE, std::vector<char>(FIELD_SIZE, '.'));
    }

    BattlefieldView::BattlefieldView(int size) {
        if (size < 10) {
            throw std::invalid_argument("Размер поля не может быть меньше 10x10");
        }
        FIELD_SIZE = size;
        field.resize(size, std::vector<char>(size, '.'));
    }

    void BattlefieldView::initField() {
        for(auto& row : field) {
            std::fill(row.begin(), row.end(), '.');
        }
    }

    void BattlefieldView::generateBasePositions(Mission& mission) {
        if (field.empty()) {
            std::cout << "Ошибка: поле не инициализировано\n";
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, FIELD_SIZE - 1);

        // Генерируем позицию для базы A
        int baseAX = dis(gen);
        int baseAY = dis(gen);

        // Генерируем позицию для базы B с проверкой расстояния
        int baseBX, baseBY;
        do {
            baseBX = dis(gen);
            baseBY = dis(gen);

            double distance = std::sqrt(
                    std::pow(baseBX - baseAX, 2) +
                    std::pow(baseBY - baseAY, 2)
            );

            if (distance >= 5) break;
        } while (true);

        mission.setBaseACoordinates({static_cast<double>(baseAX),
                                    static_cast<double>(baseAY)});
        mission.setBaseBCoordinates({static_cast<double>(baseBX),
                                     static_cast<double>(baseBY)});
    }

    void BattlefieldView::updateShipPositions
    (Mission& mission, ship::coordinate baseA, ship::coordinate baseB) {
        double distance = std::sqrt(
                std::pow(baseB.first - baseA.first, 2) +
                std::pow(baseB.second - baseA.second, 2)
        );

        if (distance < 5) {
            std::cout << "Ошибка: базы должны находиться на расстоянии не менее 5 клеток друг от друга\n";
            return;
        }

        mission.setBaseACoordinates(baseA);
        mission.setBaseBCoordinates(baseB);

        updateShipPositions(mission);
    }

    void BattlefieldView::updateShipPositions(Mission& mission) {
        // Clear the field before updating
        initField();

        auto baseA = mission.getBaseACoordinates();
        auto baseB = mission.getBaseBCoordinates();

        // Mark base A
        int xA = static_cast<int>(baseA.first) % FIELD_SIZE;
        int yA = static_cast<int>(baseA.second) % FIELD_SIZE;
        int sizeA = static_cast<int>(mission.getSizeBaseA());
        if (sizeA <= 0) sizeA = 1;

        for(int i = 0; i < sizeA && (xA + i) < FIELD_SIZE; i++) {
            for(int j = 0; j < sizeA && (yA + j) < FIELD_SIZE; j++) {
                if (xA + i >= 0 && yA + j >= 0) {
                    field[yA + j][xA + i] = 'a';
                }
            }
        }

        // Mark base B
        int xB = static_cast<int>(baseB.first) % FIELD_SIZE;
        int yB = static_cast<int>(baseB.second) % FIELD_SIZE;
        int sizeB = static_cast<int>(mission.getSizeBaseB());
        if (sizeB <= 0) sizeB = 1;

        for(int i = 0; i < sizeB && (xB + i) < FIELD_SIZE; i++) {
            for(int j = 0; j < sizeB && (yB + j) < FIELD_SIZE; j++) {
                if (xB + i >= 0 && yB + j >= 0) {
                    field[yB + j][xB + i] = 'b';
                }
            }
        }

        // Update ship positions
        const auto& ships = mission.getShipGroupTable();
        auto iter = ships.getIterator();

        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            auto pos = ship->getCurrentCoordinates();
            int x = static_cast<int>(pos.first) % FIELD_SIZE;
            int y = static_cast<int>(pos.second) % FIELD_SIZE;

            // Check field boundaries
            if (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE) {
                // Use different symbols for different ship types
                char symbol;
                switch(ship->getShipType()) {
                    case Ship::shiptype::AIRCRAFTCARRIER: symbol = 'A'; break;
                    case Ship::shiptype::CRUISER: symbol = 'C'; break;
                    case Ship::shiptype::AVIATORCRUISER: symbol = 'V'; break;
                    default: symbol = 'S';
                }

                // Check if position is already occupied
                if (field[y][x] != '.') {
                    // Generate new coordinates if position is occupied
                    bool found = false;
                    for(int i = -1; i <= 1 && !found; i++) {
                        for(int j = -1; j <= 1 && !found; j++) {
                            int newX = x + i;
                            int newY = y + j;
                            if (newX >= 0 && newX < FIELD_SIZE &&
                                newY >= 0 && newY < FIELD_SIZE &&
                                field[newY][newX] == '.') {
                                x = newX;
                                y = newY;
                                found = true;
                            }
                        }
                    }
                    if (!found) continue; // Skip if no free position found
                }
                field[y][x] = symbol;
            }
            iter.next();
        }
    }

    void BattlefieldView::displayField() const {
        for(const auto& row : field) {
            for(char cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << '\n';
        }
    }

    void BattlefieldView::displayFieldWithCoordinates() const {
        // Вывод верхней координатной строки
        std::cout << "   ";
        for(int i = 0; i < FIELD_SIZE; i++) {
            std::cout << i << " ";
            if(i < 10) std::cout << " ";
        }
        std::cout << "\n";
        // Вывод поля с боковыми координатами
        for(int i = 0; i < FIELD_SIZE; i++) {
            if(i < 10) std::cout << " ";
            std::cout << i << " ";

            for(int j = 0; j < FIELD_SIZE; j++) {
                std::cout << field[i][j] << "  ";
            }
            std::cout << "\n";
        }
    }
    void BattlefieldView::displayShipInfo(const Ship* ship, const std::string& callSign, bool fullinfo) {
        if (ship) {
            std::cout << "Корабль:   " << callSign << "\n";
            if (fullinfo) std::cout << "Название:  " << ship->getName() << "\n";
            std::cout << "Тип:       ";
            switch(ship->getShipType()) {
                case Ship::shiptype::AIRCRAFTCARRIER:
                    std::cout << "Авианосец\n";
                    if (auto carrier = dynamic_cast<const IAircraftCarrier*>(ship)) {
                        if (fullinfo) std::cout << " Макс. количество самолетов: "
                                                << carrier->getMaxAircraftCapacity() << "\n";
                        ship::airvector aircrafts = carrier->getAircrafts();
                        std::cout << " Текущие самолеты (" << aircrafts.size() << "):\n";
                        for (const auto& [aircraft, _] : aircrafts) {
                            std::cout << "   - " << (aircraft.getType() == Aircraft::AircraftType::FIGHTER ?
                                                     "Истребитель" : "Бомбардировщик") << "\n";
                            if (fullinfo) {
                                displayAircraftInfo(aircraft);
                            }
                        }
                    }
                    break;
                case Ship::shiptype::CRUISER:
                    std::cout << "Крейсер\n";
                    if (auto cruiser = dynamic_cast<const ICruiser*>(ship)) {
                        if (fullinfo) std::cout << " Макс. вместимость вооружения: " << cruiser->getMaxArmamentCapacity() << "\n";
                        if (fullinfo) std::cout << " Вместимость склада: " << cruiser->getStorageCapacity() << "\n";
                        ship::armvector weapons = cruiser->getArmament();
                        std::cout << " Вооружение (" << weapons.size() << "):\n";
                        for (const auto& weapon : weapons) {
                            std::cout << "  - " << weapon.getName() << "\n";
                            if (fullinfo) {
                                displayWeaponInfo(weapon);
                            }
                        }
                    }
                    break;
                case Ship::shiptype::AVIATORCRUISER:
                    std::cout << "Авианесущий крейсер\n";
                    if (auto aviator = dynamic_cast<const IAviatorCruiser*>(ship)) {
                        if (fullinfo) std::cout << " Макс. вместимость вооружения: " << aviator->getMaxArmamentCapacity() << "\n";
                        if (fullinfo) std::cout << " Вместимость склада: " << aviator->getStorageCapacity() << "\n";
                        if (fullinfo) std::cout << " Макс. количество самолетов: " << aviator->getMaxAircraftCapacity() << "\n";
                        ship::armvector weapons = aviator->getArmament();
                        std::cout << " Вооружение (" << weapons.size() << "):\n";
                        for (const auto& weapon : weapons) {
                            std::cout << "  - " << weapon.getName() << "\n";
                            if (fullinfo) {
                                displayWeaponInfo(weapon);
                            }
                        }
                        ship::airvector aircrafts = aviator->getAircrafts();
                        std::cout << " Текущие самолеты (" << aircrafts.size() << "):\n";
                        for (const auto& [aircraft, _] : aircrafts) {
                            std::cout << "   - " << (aircraft.getType() == Aircraft::AircraftType::FIGHTER ?
                                                     "Истребитель" : "Бомбардировщик") << "\n";
                            if (fullinfo) {
                                displayAircraftInfo(aircraft);
                            }
                        }
                    }
                    break;
                default:
                    std::cout << "Неизвестный тип корабля, добавьте функционал\n";
                    break;
            }

            if (fullinfo) {
                std::cout << "Капитан:   " << ship->getCaptainRank() << " " << ship->getCaptainName() << "\n";
                std::cout << "Прочность: " << ship->getDurability() << "\n";
                std::cout << "Скорость:  " << ship->getSpeed() << "\n";
                std::cout << "Стоимость: " << ship->getCost() << "\n";
                auto coords = ship->getCurrentCoordinates();
                std::cout << "Текущие координаты: (" << coords.first << ", " << coords.second << ")\n";
            }
        }
    }

    void BattlefieldView::displayShipsInfo(const std::string& callSign, const Mission& mission, bool fullinfo) {
        const auto& shipTable = mission.getShipGroupTable();
        const auto& ship = shipTable.getShip(callSign);
        displayShipInfo(ship, callSign, fullinfo);
    }

    void BattlefieldView::displayShipsInfo(const ship::coordinate& coords, const Mission& mission, bool fullinfo) {
        const auto& ships = mission.getShipGroupTable();
        auto iter = ships.getIterator();

        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            auto shipCoords = ship->getCurrentCoordinates();
            if (shipCoords.first == coords.first && shipCoords.second == coords.second) {
                displayShipsInfo(callSign, mission, fullinfo);
                return;
            }
            iter.next();
        }
        std::cout << "Корабль по координатам (" << coords.first << ", " << coords.second << ") не найден\n";
    }

    void BattlefieldView::displayAircraftInfo(const Aircraft& aircraft) {
        std::cout << "Информация о самолёте:\n";
        std::cout << "Тип: ";
        switch(aircraft.getType()) {
            case Aircraft::AircraftType::FIGHTER:
                std::cout << "Истребитель\n";
                break;
            case Aircraft::AircraftType::ATTACK:
                std::cout << "Бомбардировщик\n";
                break;
        }
        std::cout << "Урон: " << aircraft.getDamage() << "\n";
        std::cout << "Состояние: " << (aircraft.getActive() ? "Активен" : "Неактивен") << "\n";
        std::cout << "Прочность: " << aircraft.getDurability() << "\n";
        std::cout << "Скорость: " << aircraft.getSpeed() << "\n";
        std::cout << "Расход топлива: " << aircraft.getFuelConsumption() << "\n";
        std::cout << "Запас топлива: " << aircraft.getFuelCapacity() << "\n";
        std::cout << "Скорость заправки: " << aircraft.getRefuelSpeed() << "\n";
        std::cout << "Стоимость: " << aircraft.getCost() << "\n";
        std::cout << "Радиус атаки: " << aircraft.getAttackRadius() << "\n";
        std::cout << "Эффективный радиус атаки: " << aircraft.getEffectiveAttackRadius() << "\n";
    }

    void BattlefieldView::displayWeaponInfo(const Armament& weapon) {
        std::cout << "Информация о вооружении:\n";
        std::cout << "Название: " << weapon.getName() << "\n";
        std::cout << "Тип: ";
        switch(weapon.getType()) {
            case Armament::ArmamentType::LIGHT:
                std::cout << "Легкое\n";
                break;
            case Armament::ArmamentType::HEAVY:
                std::cout << "Тяжелое\n";
                break;
        }
        std::cout << "Тип боеприпасов: " << weapon.getAmmoName() << "\n";
        std::cout << "Состояние: " << (weapon.getActive() ? "Активно" : "Неактивно") << "\n";
        std::cout << "Урон: " << weapon.getDamage() << "\n";
        std::cout << "Дальность стрельбы: " << weapon.getRangeOfFire() << "\n";
        std::cout << "Скорострельность: " << weapon.getRateOfFire() << "\n";
        std::cout << "Макс. боезапас: " << weapon.getMaxAmmoCapacity() << "\n";
        std::cout << "Текущий боезапас: " << weapon.getCurrentAmmo() << "\n";
        std::cout << "Скорость перезарядки: " << weapon.getReloadSpeed() << "\n";
        std::cout << "Стоимость: " << weapon.getCost() << "\n";
    }

    void BattlefieldView::handleShipMovement(Mission* mission) {
        auto iter = mission->getShipGroupTable().getIterator();
        std::vector<std::string> playerShips;
        std::cout << "\nВаши корабли:\n";
        int i = 1;

        // Получаем список кораблей игрока
        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find("AI") == std::string::npos) {
                std::cout << i << ". " << callSign << " на позиции ("
                          << ship->getCurrentCoordinates().first << ","
                          << ship->getCurrentCoordinates().second << ")\n";
                playerShips.push_back(callSign);
                i++;
            }
            iter.next();
        }

        if (playerShips.empty()) {
            std::cout << "У вас нет кораблей!\n";
            return;
        }

        // Выбор корабля
        std::cout << "Выберите корабль (1-" << playerShips.size() << "): ";
        int shipChoice = Prog1::getNumber<int>(1, static_cast<int>(playerShips.size()));
        std::string selectedShip = playerShips[shipChoice-1];

        // Ввод новых координат
        std::cout << "Введите новые координаты (x y): ";
        int x = Prog1::getNumber<int>(1, 19);
        int y = Prog1::getNumber<int>(1, 19);

        Ship* ship = mission->getShipGroupTable().getShip(selectedShip);
        ship->setDestinationCoordinates({static_cast<double>(x), static_cast<double>(y)});
        ship->move();

        updateShipPositions(*mission);
    }

    void BattlefieldView::handleAttack(Mission* mission) {
        std::cout << "\nВыберите цель для атаки:\n";

        // Получаем список вражеских кораблей
        std::vector<std::pair<std::string, Ship*>> enemyShips;
        auto iter = mission->getShipGroupTable().getIterator();
        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find("AI") != std::string::npos) {
                enemyShips.emplace_back(callSign, ship);
            }
            iter.next();
        }

        // Показываем список целей
        for(size_t i = 0; i < enemyShips.size(); i++) {
            auto [callSign, ship] = enemyShips[i];
            auto pos = ship->getCurrentCoordinates();
            std::cout << i+1 << ". " << callSign << " на позиции ("
                      << pos.first << "," << pos.second << ")\n";
        }

        if(enemyShips.empty()) {
            std::cout << "Нет доступных целей\n";
            return;
        }

        // Выбор цели
        int choice = Prog1::getNumber<int>(1, static_cast<int>(enemyShips.size()));
        auto targetShip = enemyShips[choice-1];

        // Выбор атакующего корабля
        std::cout << "\nВыберите атакующий корабль:\n";
        std::vector<std::pair<std::string, Ship*>> playerShips;
        iter = mission->getShipGroupTable().getIterator();
        while(iter.hasNext()) {
            auto [callSign, ship] = iter.get();
            if (callSign.find("AI") == std::string::npos) {
                playerShips.emplace_back(callSign, ship);
            }
            iter.next();
        }

        for(size_t i = 0; i < playerShips.size(); i++) {
            auto [callSign, ship] = playerShips[i];
            std::cout << i+1 << ". " << callSign << "\n";
        }

        choice = Prog1::getNumber<int>(1, static_cast<int>(playerShips.size()));
        auto attackingShip = playerShips[choice-1];

        // Выполнение атаки в зависимости от типа корабля
        const auto targetPos = targetShip.second->getCurrentCoordinates();
        switch(attackingShip.second->getShipType()) {
            case Ship::shiptype::AIRCRAFTCARRIER:
                mission->simulateAirRaid(attackingShip.first, targetPos);
                break;
            case Ship::shiptype::AVIATORCRUISER:
                // Авианесущий крейсер применяет и авиацию, и бортовое вооружение
                mission->simulateAirRaid(attackingShip.first, targetPos);
                if (auto* cruiser = dynamic_cast<ICruiser*>(attackingShip.second))
                    cruiser->fireAtShip(targetPos);
                break;
            case Ship::shiptype::CRUISER:
                if (auto* cruiser = dynamic_cast<ICruiser*>(attackingShip.second))
                    cruiser->fireAtShip(targetPos);
                break;
        }

        updateShipPositions(*mission);
    }

} // acg