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
        std::cout <<  mission->getCount() << std::endl;
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
}






class MissionTransferTest : public ::testing::Test {
    protected:
    void SetUp() override {
        mission = new acg::Mission("Commander", 5, 1000.0);

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
    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", "TO_CARRIER"), acg::MissionError::SUCCESS);
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

TEST_F(MissionTransferTest, TransferPlaneInvalidShipType) {
    // Here you would need to set up ships that are not AircraftCarriers or AviatorCruisers
    // Assuming we have a different ship type, e.g., a Battleship
    auto *battleship = new acg::Cruiser(acg::Ship::shiptype::CRUISER,"Battleship",
                                        "3 rang", "OMEGA", 70.5,
                                        80, 100, 20, 30);
    mission->buyShip("BATTLESHIP", battleship);

    EXPECT_EQ(mission->transferPlane(aircraft, "BATTLESHIP", "TO_CARRIER"), acg::MissionError::INVALID_SHIP_TYPE);
    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", "BATTLESHIP"), acg::MissionError::INVALID_SHIP_TYPE);

    delete battleship;
}

TEST_F(MissionTransferTest, TransferPlaneStorageFull) {
    // Fill the toCarrier to its max capacity
    for (int i = 0; i < toCarrier->getMaxAircraftCapacity(); ++i) {
        auto* extraAircraft = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER, 100, true, 200, 50.0, 1000.0, 100.0, 100, 100, 4
        );
        mission->buyPlaneForShip("TO_CARRIER", extraAircraft);
    }

    EXPECT_EQ(mission->transferPlane(aircraft, "FROM_CARRIER", "TO_CARRIER"), acg::MissionError::STORAGE_FULL);

}

TEST_F(MissionTransferTest, TransferPlaneAircraftNotFound) {
    // Attempt to transfer an aircraft that is not on the fromCarrier
    auto* newAircraft = new acg::Aircraft(
            acg::Aircraft::AircraftType::FIGHTER, 100, true, 200, 50.0, 1000.0, 100.0, 100, 100, 4
    );

    EXPECT_EQ(mission->transferPlane(newAircraft, "FROM_CARRIER", "TO_CARRIER"), acg::MissionError::AIRCRAFT_NOT_FOUND);

    delete newAircraft;
}
