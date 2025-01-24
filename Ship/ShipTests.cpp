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

// Тесты для move
TEST_F(ShipTest, MoveTest) {
    acg::ship::coordinate dest = {100.0, 100.0};
    cruiser->setCurrentCoordinates({0.0, 0.0});
    cruiser->setDestinationCoordinates(dest);
    cruiser->setSpeed(50.0);

// Первый вызов (счетчик = 1)
    cruiser->move();
    auto pos1 = cruiser->getCurrentCoordinates();
    EXPECT_EQ(pos1.first, 0.0);
    EXPECT_EQ(pos1.second, 0.0);

// Второй вызов (счетчик = 2, движение)
    cruiser->move();
    auto pos2 = cruiser->getCurrentCoordinates();
    EXPECT_GT(pos2.first, 0.0);
    EXPECT_GT(pos2.second, 0.0);
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