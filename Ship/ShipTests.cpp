#include <gtest/gtest.h>
#include "Ship.h"

class ShipTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем корабли разных типов для тестирования
        cruiser = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Cruiser1", "Captain", "John", 30.0, 100, 1000.0);
        carrier = new acg::Ship(acg::Ship::shiptype::AIRCRAFTCARRIER,
                                "Carrier1", "Captain", "Jack", 25.0, 100, 2000.0);
        aviator = new acg::Ship(acg::Ship::shiptype::AVIATORCRUISER,
                                "Aviator1", "Captain", "James", 35.0, 100, 1500.0);
    }

    void TearDown() override {
        delete cruiser;
        delete carrier;
        delete aviator;
    }

    acg::Ship *cruiser{}, *carrier{}, *aviator{};
};

// Тесты для receiveDamage
TEST_F(ShipTest, ReceiveDamageTest) {
// Тест для крейсера (множитель урона 0.8)
    cruiser->receiveDamage(100);
    EXPECT_EQ(cruiser->getDurability(), 20); // 100 - (100 * 0.8)

// Тест для авианосца (множитель урона 1.2)
    carrier->receiveDamage(100);
    EXPECT_EQ(carrier->getDurability(), 0); // 100 - (100 * 1.2) < 0, поэтому 0

// Тест для авиакрейсера (множитель урона 1.0)
    aviator->receiveDamage(50);
    EXPECT_EQ(aviator->getDurability(), 50); // 100 - (50 * 1.0)
}

// Тесты для calculateTotalCost
TEST_F(ShipTest, CalculateTotalCostTest) {
// Установка начальной прочности
    cruiser->setDurability(50);
    EXPECT_DOUBLE_EQ(cruiser->calculateTotalCost(), 500.0); // 1000 * (50/100)

    carrier->setDurability(75);
    EXPECT_DOUBLE_EQ(carrier->calculateTotalCost(), 1500.0); // 2000 * (75/100)

    aviator->setDurability(25);
    EXPECT_DOUBLE_EQ(aviator->calculateTotalCost(), 375.0); // 1500 * (25/100)
}

// Тесты для setDestination
TEST_F(ShipTest, SetDestinationTest) {
    cruiser->setSpeed(10.0);
    cruiser->setCurrentCoordinates({0.0, 0.0});

// Тест на допустимую дистанцию
    EXPECT_NO_THROW(cruiser->setDestination({50.0, 50.0}));

// Тест на слишком большую дистанцию
    EXPECT_THROW(cruiser->setDestination({1000.0, 1000.0}), std::invalid_argument);
}


class ShipMoveTest : public ::testing::Test {
protected:
    void SetUp() override {
        ship = new acg::Ship(
                acg::Ship::shiptype::CRUISER,
                "TestShip", "Captain", "John",
                30.0, 100, 500.0
        );
    }

    void TearDown() override {
        delete ship;
    }

    acg::Ship* ship{};
};

// Тест движения на дальнее расстояние
TEST_F(ShipMoveTest, MoveLongDistance) {
    ship->setSpeed(10.0);
    ship->setCurrentCoordinates({0.0, 0.0});
    ship->setDestinationCoordinates({10.0, 10.0});

    ship->move();

    auto pos = ship->getCurrentCoordinates();
    EXPECT_GT(pos.first, 0.0);
    EXPECT_GT(pos.second, 0.0);
    EXPECT_LT(pos.first, 100.0);
    EXPECT_LT(pos.second, 100.0);
}

// Тест движения на короткое расстояние
TEST_F(ShipMoveTest, MoveShortDistance) {
    ship->setSpeed(50.0);
    ship->setCurrentCoordinates({0.0, 0.0});
    ship->setDestinationCoordinates({5.0, 5.0});

    ship->move();

    auto pos = ship->getCurrentCoordinates();
    EXPECT_DOUBLE_EQ(pos.first, 5.0);
    EXPECT_DOUBLE_EQ(pos.second, 5.0);
}

// Тест движения при нулевом расстоянии
TEST_F(ShipMoveTest, MoveZeroDistance) {
    ship->setSpeed(10.0);
    ship->setCurrentCoordinates({5.0, 5.0});
    ship->setDestinationCoordinates({5.0, 5.0});

    ship->move();

    auto pos = ship->getCurrentCoordinates();
    EXPECT_DOUBLE_EQ(pos.first, 5.0);
    EXPECT_DOUBLE_EQ(pos.second, 5.0);
}