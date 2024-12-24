#include <gtest/gtest.h>
#include "Aircraft.h"

class AircraftTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовый самолет с валидными параметрами
        aircraft = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER,
                100,    // damage
                true,   // active
                100,    // durability
                50.0,   // speed
                10.0,   // fuel_consumption
                1000.0, // fuel_capacity
                20.0,   // refuel_speed
                5000.0, // cost
                100.0   // attack_radius
        );
    }

    void TearDown() override {
        delete aircraft;
    }

    acg::Aircraft* aircraft{};
};

// Тест конструктора с параметрами
TEST_F(AircraftTest, ConstructorTest) {
    EXPECT_EQ(aircraft->getType(), acg::Aircraft::AircraftType::FIGHTER);
    EXPECT_EQ(aircraft->getDamage(), 100);
    EXPECT_TRUE(aircraft->getActive());
    EXPECT_EQ(aircraft->getDurability(), 100);
    EXPECT_DOUBLE_EQ(aircraft->getSpeed(), 50.0);
    EXPECT_DOUBLE_EQ(aircraft->getFuelConsumption(), 10.0);
    EXPECT_DOUBLE_EQ(aircraft->getFuelCapacity(), 1000.0);
    EXPECT_DOUBLE_EQ(aircraft->getRefuelSpeed(), 20.0);
    EXPECT_DOUBLE_EQ(aircraft->getCost(), 5000.0);
    EXPECT_DOUBLE_EQ(aircraft->getAttackRadius(), 100.0);
}

// Тест на исключения в конструкторе
TEST_F(AircraftTest, ConstructorExceptions) {
    EXPECT_THROW(acg::Aircraft(
            acg::Aircraft::AircraftType::FIGHTER,
            -100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 100.0
    ), std::invalid_argument);
}

// Тест метода makeAttackRun
TEST_F(AircraftTest, MakeAttackRunTest) {
    double initial_fuel = aircraft->getFuelCapacity();
    aircraft->makeAttackRun(50.0);
    EXPECT_LT(aircraft->getFuelCapacity(), initial_fuel);
    EXPECT_LT(aircraft->getDurability(), 100);
}

// Тест метода receiveDamage
TEST_F(AircraftTest, ReceiveDamageTest) {
    double initial_cost = aircraft->getCost();
    aircraft->receiveDamage(50);
    EXPECT_EQ(aircraft->getDurability(), 50);
    EXPECT_LT(aircraft->getCost(), initial_cost);
    EXPECT_LT(aircraft->getSpeed(), 50.0);
}

// Тест метода getEffectiveAttackRadius
TEST_F(AircraftTest, EffectiveAttackRadiusTest) {
    double radius = aircraft->getEffectiveAttackRadius();
    EXPECT_GT(radius, 0.0);

    // Тест при неактивном состоянии
    aircraft->setActive(false);
    EXPECT_DOUBLE_EQ(aircraft->getEffectiveAttackRadius(), 0.0);

    // Тест при нулевой прочности
    aircraft->setActive(true);
    aircraft->setDurability(0);
    EXPECT_DOUBLE_EQ(aircraft->getEffectiveAttackRadius(), 0.0);
}

// Тест оператора равенства
TEST_F(AircraftTest, EqualityOperatorTest) {
    acg::Aircraft aircraft2(
            acg::Aircraft::AircraftType::FIGHTER,
            100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 100.0
    );
    EXPECT_TRUE(*aircraft == aircraft2);
}
