#include <gtest/gtest.h>
#include "Mission.h"

class MissionTest : public ::testing::Test {
protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 1000.0);
    }

    void TearDown() override {
        delete mission;
    }

    acg::Mission* mission{};
};

// Тест проверки бюджета
TEST_F(MissionTest, CheckBudget) {
    EXPECT_TRUE(mission->checkBudget(500.0));
    EXPECT_FALSE(mission->checkBudget(1500.0));
}

// Тест получения оставшегося бюджета
TEST_F(MissionTest, GetRemainingBudget) {
    EXPECT_DOUBLE_EQ(mission->getRemainingBudget(), 1000.0);
    mission->setSpentSum(300.0);
    EXPECT_DOUBLE_EQ(mission->getRemainingBudget(), 700.0);
}

// Тест покупки корабля
TEST_F(MissionTest, BuyShip) {
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER, "Test", "Captain", "John", 30.0, 100, 500.0);

    // Проверка пустого позывного
    EXPECT_EQ(mission->buyShip("", ship), acg::MissionError::EMPTY_CALLSIGN);

    // Проверка nullptr
    EXPECT_EQ(mission->buyShip("TEST1", nullptr), acg::MissionError::SHIP_NOT_FOUND);

    // Успешная покупка
    EXPECT_EQ(mission->buyShip("TEST1", ship), acg::MissionError::SUCCESS);

    // Проверка превышения лимита кораблей
    for(int i = 0; i < 5; i++) {
        auto* extra_ship = new acg::Ship(acg::Ship::shiptype::CRUISER, "Test", "Captain", "John", 30.0, 100, 100.0);
        mission->buyShip("TEST" + std::to_string(i), extra_ship);
    }
    EXPECT_EQ(mission->buyShip("TEST6", ship), acg::MissionError::MAX_SHIPS_REACHED);
}

// Тест продажи корабля
TEST_F(MissionTest, SellShip) {
    // Проверка пустого позывного
    EXPECT_EQ(mission->sellShip(""), acg::MissionError::EMPTY_CALLSIGN);

    // Проверка несуществующего корабля
    EXPECT_EQ(mission->sellShip("TEST1"), acg::MissionError::SHIP_NOT_FOUND);

    // Проверка успешной продажи
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER, "Test", "Captain", "John", 30.0, 100, 500.0);
    mission->buyShip("TEST1", ship);
    EXPECT_EQ(mission->sellShip("TEST1"), acg::MissionError::SUCCESS);
}


class MissionAircraftTest : public ::testing::Test {
protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 1000.0);

        // Создаем тестовый корабль-авианосец
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 500.0
        );
        carrier->setMaxAircraftCapacity(5);

        // Создаем тестовый самолет
        aircraft = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER, 100, true, 200.0, 50, 1000.0, 100.0, 100, 100, 4
        );

        mission->buyShip("CARRIER1", carrier);
    }

    void TearDown() override {
        delete mission;
        delete aircraft;
    }

    acg::Mission* mission{};
    acg::AircraftCarrier* carrier{};
    acg::Aircraft* aircraft{};
};

// Тест покупки самолета
TEST_F(MissionAircraftTest, BuyPlaneForShip) {
    // Проверка с пустым позывным
    EXPECT_EQ(mission->buyPlaneForShip("", aircraft), acg::MissionError::EMPTY_CALLSIGN);

    // Проверка с nullptr самолета
    EXPECT_EQ(mission->buyPlaneForShip("CARRIER1", nullptr), acg::MissionError::AIRCRAFT_NOT_FOUND);

    // Проверка с несуществующим кораблем
    EXPECT_EQ(mission->buyPlaneForShip("NONEXISTENT", aircraft), acg::MissionError::SHIP_NOT_FOUND);

    // Проверка успешной покупки
    EXPECT_EQ(mission->buyPlaneForShip("CARRIER1", aircraft), acg::MissionError::SUCCESS);

    // Проверка превышения вместимости
    auto* extra_aircraft = new acg::Aircraft(
            acg::Aircraft::AircraftType::FIGHTER, 100, true, 200.0, 50, 1000.0, 100.0, 100, 100, 4
    );
    for(int i = 0; i < 5; i++) {
        mission->buyPlaneForShip("CARRIER1", extra_aircraft);
    }
    EXPECT_EQ(mission->buyPlaneForShip("CARRIER1", extra_aircraft),
              acg::MissionError::INSUFFICIENT_FUNDS);
    delete extra_aircraft;
}

// Тест продажи самолета
TEST_F(MissionAircraftTest, SellPlaneFromShip) {
    // Предварительная покупка самолета
    EXPECT_EQ(mission->buyPlaneForShip("CARRIER1", aircraft), acg::MissionError::SUCCESS);

    // Проверка с пустым позывным
    EXPECT_EQ(mission->sellPlaneFromShip("", aircraft), acg::MissionError::EMPTY_CALLSIGN);

    // Проверка с nullptr самолета
    EXPECT_EQ(mission->sellPlaneFromShip("CARRIER1", nullptr),
              acg::MissionError::AIRCRAFT_NOT_FOUND);

    // Проверка с несуществующим кораблем
    EXPECT_EQ(mission->sellPlaneFromShip("NONEXISTENT", aircraft),
              acg::MissionError::SHIP_NOT_FOUND);

    EXPECT_EQ(mission->sellPlaneFromShip("CARRIER1", aircraft),
              acg::MissionError::SUCCESS);

    auto* newPlane = new acg::Aircraft(acg::Aircraft::AircraftType::FIGHTER, 100, true, 200.0, 50, 1000.0, 100.0, 100, 100, 6);
    EXPECT_EQ(mission->buyPlaneForShip("CARRIER1", newPlane), acg::MissionError::SUCCESS);
    delete newPlane;
}

TEST_F(MissionAircraftTest, SellPlaneFromShipSuccess) {
    // Подготовка: покупаем самолёт для корабля
    auto* testAircraft = new acg::Aircraft(
            acg::Aircraft::AircraftType::FIGHTER,
            100, true, 200.0, 50, 1000.0,
            100.0, 100, 100, 4
    );
    ASSERT_EQ(mission->buyPlaneForShip("CARRIER1", testAircraft),
              acg::MissionError::SUCCESS);

    // Проверяем успешную продажу самолёта
    EXPECT_EQ(mission->sellPlaneFromShip("CARRIER1", testAircraft),
              acg::MissionError::SUCCESS);
    delete testAircraft;
}


class MissionTransferTest : public ::testing::Test {
    protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 100000.0);

        fromCarrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "FromCarrier", "Captain", "John", 30.0, 100, 500.0
        );
        fromCarrier->setMaxAircraftCapacity(5);

        toCarrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "ToCarrier", "Captain", "Doe", 30.0, 100, 500.0
        );
        toCarrier->setMaxAircraftCapacity(5);

        aircraft = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER, 100, true, 200, 50.0, 1000.0, 100.0, 100, 100, 4
        );

        mission->buyShip("FROM_CARRIER", fromCarrier);
        mission->buyShip("TO_CARRIER", toCarrier);
        mission->buyPlaneForShip("FROM_CARRIER", aircraft);
    }

    void TearDown() override {
        delete mission;
        delete aircraft;
    }

    acg::Mission* mission{};
    acg::AircraftCarrier* fromCarrier{};
    acg::AircraftCarrier* toCarrier{};
    acg::Aircraft* aircraft{};
};

TEST_F(MissionTransferTest, TransferPlaneSuccess) {
    // Проверяем, что самолёт действительно есть на исходном корабле
    ASSERT_TRUE(fromCarrier != nullptr);
    ASSERT_TRUE(fromCarrier->getShipType() == acg::Ship::shiptype::AIRCRAFTCARRIER);
    ASSERT_TRUE(!(fromCarrier->getAircrafts().empty()));

    // Выполняем перемещение
    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", "TO_CARRIER"),
              acg::MissionError::SUCCESS);

    ASSERT_TRUE(toCarrier != nullptr);
    ASSERT_TRUE(!(toCarrier->getAircrafts().empty()));
}

TEST_F(MissionTransferTest, TransferPlaneEmptyCallsign) {
    EXPECT_EQ(mission->transferPlane(aircraft, "", "TO_CARRIER"), acg::MissionError::EMPTY_CALLSIGN);
    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", ""), acg::MissionError::EMPTY_CALLSIGN);
}

TEST_F(MissionTransferTest, TransferPlaneNullAircraft) {
    EXPECT_EQ(mission->transferPlane(nullptr, "FROM_CARRIER", "TO_CARRIER"), acg::MissionError::AIRCRAFT_NOT_FOUND);
}

TEST_F(MissionTransferTest, TransferPlaneShipNotFound) {
    EXPECT_EQ(mission->transferPlane(aircraft, "NON_EXISTENT", "TO_CARRIER"), acg::MissionError::SHIP_NOT_FOUND);
    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", "NON_EXISTENT"), acg::MissionError::SHIP_NOT_FOUND);
}

TEST_F(MissionTransferTest, TransferPlaneAircraftNotFound) {
    // Attempt to transfer an aircraft that is not on the fromCarrier
    auto* newAircraft = new acg::Aircraft(
            acg::Aircraft::AircraftType::FIGHTER, 8, true, 200, 50.0, 1000.0, 100.0, 100, 100, 4
    );

    EXPECT_EQ(mission->transferPlane(newAircraft, "FROM_CARRIER", "TO_CARRIER"), acg::MissionError::AIRCRAFT_NOT_FOUND);

    delete newAircraft;
}


class MissionWeaponTest : public ::testing::Test {
protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 100000.0);

        // Создаем тестовый крейсер
        cruiser = new acg::Cruiser(acg::Ship::shiptype::CRUISER,
                                   "TestCruiser", "Captain", "John",
                                   30.0, 100, 500.0, 5, 1000);

        // Создаем тестовое оружие
        weapon = new acg::Armament("TestWeapon", acg::Armament::ArmamentType::LIGHT,
                                   100, 1000.0, 60.0, 100, 2.0, 200.0);

        mission->buyShip("CRUISER1", cruiser);
    }

    void TearDown() override {
        delete mission;
        delete weapon;
    }

    acg::Mission* mission{};
    acg::Cruiser* cruiser{};
    acg::Armament* weapon{};
};

// Тесты для destroyShip
TEST_F(MissionWeaponTest, DestroyShipEmptyCallsign) {
    EXPECT_EQ(mission->destroyShip(""), acg::MissionError::EMPTY_CALLSIGN);
}

TEST_F(MissionWeaponTest, DestroyShipNotFound) {
    EXPECT_EQ(mission->destroyShip("NONEXISTENT"), acg::MissionError::SHIP_NOT_FOUND);
}

TEST_F(MissionWeaponTest, DestroyShipSuccess) {
    EXPECT_EQ(mission->destroyShip("CRUISER1"), acg::MissionError::SUCCESS);
}

// Тесты для buyWeaponForShip
TEST_F(MissionWeaponTest, BuyWeaponEmptyCallsign) {
    EXPECT_EQ(mission->buyWeaponForShip("", weapon), acg::MissionError::EMPTY_CALLSIGN);
}

TEST_F(MissionWeaponTest, BuyWeaponNullWeapon) {
    EXPECT_EQ(mission->buyWeaponForShip("CRUISER1", nullptr),
              acg::MissionError::WEAPON_NOT_FOUND);
}

TEST_F(MissionWeaponTest, BuyWeaponShipNotFound) {
    EXPECT_EQ(mission->buyWeaponForShip("NONEXISTENT", weapon),
              acg::MissionError::SHIP_NOT_FOUND);
}

TEST_F(MissionWeaponTest, BuyWeaponInsufficientFunds) {
    auto* expensiveWeapon = new acg::Armament("ExpensiveWeapon",
                                              acg::Armament::ArmamentType::LIGHT,
                                              100, 1000.0, 60.0, 100, 2.0, 20000000.0);
    EXPECT_EQ(mission->buyWeaponForShip("CRUISER1", expensiveWeapon),
              acg::MissionError::INSUFFICIENT_FUNDS);
    delete expensiveWeapon;
}

TEST_F(MissionWeaponTest, BuyWeaponInvalidShipType) {
    auto* aicarrier = new acg::AircraftCarrier(acg::Ship::shiptype::AIRCRAFTCARRIER,
                                             "TestCarrier", "Captain", "Jack",
                                             25.0, 100, 800.0);
    mission->buyShip("CARRIER3", aicarrier);
    EXPECT_EQ(mission->buyWeaponForShip("CARRIER3", weapon),
              acg::MissionError::INVALID_SHIP_TYPE);
}

TEST_F(MissionWeaponTest, BuyWeaponSuccess) {
    EXPECT_EQ(mission->buyWeaponForShip("CRUISER1", weapon),
              acg::MissionError::SUCCESS);
    // Проверяем, что оружие добавлено
    auto armament = cruiser->getArmament();
    EXPECT_EQ(armament.size(), 1);
}

TEST_F(MissionWeaponTest, BuyWeaponStorageFull) {
    // Заполняем хранилище оружия
    std::cout << "Max capacity: " << cruiser->getMaxArmamentCapacity() << std::endl;
    for(int i = 0; i < cruiser->getMaxArmamentCapacity(); i++) {
        mission->buyWeaponForShip("CRUISER1", weapon);
    }
    // Пытаемся добавить ещё одно оружие
    auto* extraWeapon = new acg::Armament("ExtraWeapon",
                                          acg::Armament::ArmamentType::LIGHT,
                                          100, 1000.0, 60.0, 100, 2.0, 100.0);
    EXPECT_EQ(mission->buyWeaponForShip("CRUISER1", extraWeapon),
              acg::MissionError::STORAGE_FULL);
    delete extraWeapon;
}


TEST_F(MissionWeaponTest, SellWeaponEmptyCallsign) {
    auto* weapon = new acg::Armament("TestWeapon", acg::Armament::ArmamentType::LIGHT,
                                     100, 1000.0, 60.0, 100, 2.0, 200.0);
    EXPECT_EQ(mission->sellWeaponFromShip("", weapon), acg::MissionError::EMPTY_CALLSIGN);
    delete weapon;
}

TEST_F(MissionWeaponTest, SellWeaponNullWeapon) {
    EXPECT_EQ(mission->sellWeaponFromShip("CRUISER1", nullptr),
              acg::MissionError::WEAPON_NOT_FOUND);
}

TEST_F(MissionWeaponTest, SellWeaponShipNotFound) {
    auto* weapon = new acg::Armament("TestWeapon", acg::Armament::ArmamentType::LIGHT,
                                     100, 1000.0, 60.0, 100, 2.0, 200.0);
    EXPECT_EQ(mission->sellWeaponFromShip("NONEXISTENT", weapon),
              acg::MissionError::SHIP_NOT_FOUND);
    delete weapon;
}

TEST_F(MissionWeaponTest, SellWeaponInvalidShipType) {
    auto* carrier = new acg::AircraftCarrier(acg::Ship::shiptype::AIRCRAFTCARRIER,
                                             "Carrier", "Captain", "Jack", 25.0, 100, 800.0);
    auto* weapon = new acg::Armament("TestWeapon", acg::Armament::ArmamentType::LIGHT,
                                     100, 1000.0, 60.0, 100, 2.0, 200.0);
    mission->buyShip("CARRIER1", carrier);
    EXPECT_EQ(mission->sellWeaponFromShip("CARRIER1", weapon),
              acg::MissionError::INVALID_SHIP_TYPE);

    delete weapon;
}

TEST_F(MissionWeaponTest, SellWeaponSuccess) {
    auto* weapon = new acg::Armament("TestWeapon", acg::Armament::ArmamentType::LIGHT,
                                     100, 1000.0, 60.0, 100, 2.0, 200.0);
    mission->buyWeaponForShip("CRUISER1", weapon);
    EXPECT_EQ(mission->sellWeaponFromShip("CRUISER1", weapon),
              acg::MissionError::SUCCESS);
    // Проверяем возврат средств
    double expected_refund = weapon->getCost() * 0.7;
    EXPECT_DOUBLE_EQ(mission->getRemainingBudget() + 500,
                     mission->getBudget() - weapon->getCost() + expected_refund);

    delete weapon;
}

TEST_F(MissionTest, MarkEnemyAsReachedEmptyCallsign) {
    // Тест с пустым позывным
    EXPECT_EQ(mission->markEnemyAsReached(""), acg::MissionError::EMPTY_CALLSIGN);
}

TEST_F(MissionTest, MarkEnemyAsReachedShipNotFound) {
    // Тест с несуществующим кораблем
    EXPECT_EQ(mission->markEnemyAsReached("NONEXISTENT"), acg::MissionError::SHIP_NOT_FOUND);
}

TEST_F(MissionTest, MarkEnemyAsReachedSuccess) {
    // Создаем и добавляем тестовый корабль
    auto* ship = new acg::Ship(
            acg::Ship::shiptype::CRUISER,
            "TestShip", "Captain", "John",
            30.0, 100, 1000.0
    );
    mission->buyShip("TEST1", ship);

    // Проверяем успешное выполнение
    EXPECT_EQ(mission->markEnemyAsReached("TEST1"), acg::MissionError::SUCCESS);

    // Проверяем, что стоимость спасенных кораблей увеличилась
    EXPECT_GT(mission->getSavedUnitsCost(), 0.0);
}

TEST_F(MissionTest, MarkEnemyAsReachedWithDamage) {
    // Создаем поврежденный корабль
    auto* ship = new acg::Ship(
            acg::Ship::shiptype::CRUISER,
            "TestShip", "Captain", "John",
            30.0, 50, 1000.0  // 50% прочности
    );
    mission->buyShip("TEST1", ship);

    double initial_cost = ship->calculateTotalCost();
    mission->markEnemyAsReached("TEST1");

    // Проверяем, что стоимость спасенных кораблей учитывает повреждения
    EXPECT_DOUBLE_EQ(mission->getSavedUnitsCost(), initial_cost * 0.5);
}






class MissionAirRaidTest : public ::testing::Test {
protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 100000.0);

        // Создаем авианосец
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 500.0
        );
        carrier->setMaxAircraftCapacity(5);

        // Создаем самолеты
        fighter = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER,
                100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 100.0
        );

        bomber = new acg::Aircraft(
                acg::Aircraft::AircraftType::ATTACK,
                100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 100.0
        );

        mission->buyShip("CARRIER1", carrier);
    }

    void TearDown() override {
        delete mission;
        delete fighter;
        delete bomber;
    }

    acg::Mission* mission{};
    acg::AircraftCarrier* carrier{};
    acg::Aircraft *fighter{}, *bomber{};
};

// Тест на пустой позывной
TEST_F(MissionAirRaidTest, EmptyCallsign) {
    acg::ship::coordinate target = {100.0, 100.0};
    EXPECT_EQ(mission->simulateAirRaid("", target),
              acg::MissionError::EMPTY_CALLSIGN);
}

// Тест на несуществующий корабль
TEST_F(MissionAirRaidTest, ShipNotFound) {
    acg::ship::coordinate target = {100.0, 100.0};
    EXPECT_EQ(mission->simulateAirRaid("NONEXISTENT", target),
              acg::MissionError::SHIP_NOT_FOUND);
}

// Тест на неверный тип корабля
TEST_F(MissionAirRaidTest, InvalidShipType) {
    auto* cruiser = new acg::Cruiser(
            acg::Ship::shiptype::CRUISER,
            "TestCruiser", "Captain", "Jack", 30.0, 100, 500.0, 5, 1000
    );
    mission->buyShip("CRUISER1", cruiser);

    acg::ship::coordinate target = {100.0, 100.0};
    EXPECT_EQ(mission->simulateAirRaid("CRUISER1", target), acg::MissionError::INVALID_SHIP_TYPE);
}

// Тест успешного налета
TEST_F(MissionAirRaidTest, SuccessfulRaid) {
    // Добавляем самолеты на авианосец
    acg::ship::airvector aircraft = {
            {*fighter, {0.0, 0.0}},
            {*bomber, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(aircraft);

    acg::ship::coordinate target = {100.0, 100.0};
    EXPECT_EQ(mission->simulateAirRaid("CARRIER1", target), acg::MissionError::SUCCESS);
}

// Тест для сравнения производительности однопоточной и многопоточной версий
TEST_F(MissionTest, ComparePerformance) {
    mission ("Commander", 100000, 1000000.0);
    auto* carrier = new AircraftCarrier(
            Ship::shiptype::AIRCRAFTCARRIER,
            "TestCarrier", "Captain", "John", 30.0, 100, 500.0
    );
    mission.buyShip("CARRIER1", carrier);

    std::vector<size_t> test_sizes = {1000, 5000, 10000, 25000};

    for(auto size : test_sizes) {
        // Добавляем самолеты разных типов
        for(int i = 0; i < size; i++) {
            auto* fighter = new Aircraft(
                    Aircraft::AircraftType::FIGHTER,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 1000.0
            );
            auto* bomber = new Aircraft(
                    Aircraft::AircraftType::ATTACK,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 1000.0
            );
            mission.buyPlaneForShip("CARRIER1", fighter);
            mission.buyPlaneForShip("CARRIER1", bomber);
        }

        // Замеряем время однопоточной версии
        auto start_single = std::chrono::high_resolution_clock::now();
        mission.simulateAirRaid("CARRIER1", {1000.0, 1000.0});
        auto end_single = std::chrono::high_resolution_clock::now();
        auto duration_single = std::chrono::duration_cast<std::chrono::milliseconds>
                (end_single - start_single).count();

        // Замеряем время многопоточной версии
        auto start_multi = std::chrono::high_resolution_clock::now();
        mission.MULTIsimulateAirRaid("CARRIER1", {1000.0, 1000.0});
        auto end_multi = std::chrono::high_resolution_clock::now();
        auto duration_multi = std::chrono::duration_cast<std::chrono::milliseconds>
                (end_multi - start_multi).count();

        std::cout << "Size: " << size << "\n"
                  << "Single-threaded: " << duration_single << "ms\n"
                  << "Multi-threaded: " << duration_multi << "ms\n"
                  << "Speedup: " << static_cast<double>(duration_single)/duration_multi << "x\n\n";
    }
}
