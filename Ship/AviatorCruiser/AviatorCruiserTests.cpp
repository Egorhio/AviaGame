#include <gtest/gtest.h>
#include "AviatorCruiser.h"

class AviatorCruiserBomberTest : public ::testing::Test {
protected:
    void SetUp() override {
        aviator = new acg::AviatorCruiser(
                acg::Ship::shiptype::AVIATORCRUISER,
                "TestAviator", "Captain", "John",
                30.0, 100, 500.0, 5, 1000, 5
        );

        // Создаем тестовый бомбардировщик
        bomber = new acg::Aircraft(
                acg::Aircraft::AircraftType::ATTACK,
                100, true, 100, 50.0, 10.0,
                1000.0, 20.0, 500.0, 1000.0
        );
    }

    void TearDown() override {
        delete aviator;
        delete bomber;
    }

    acg::AviatorCruiser* aviator{};
    acg::Aircraft* bomber{};
};

// Тест атаки без самолетов
TEST_F(AviatorCruiserBomberTest, EmptyBomberAttack) {
    acg::ship::coordinate target = {1000.0, 1000.0};
    aviator->setCurrentCoordinates({0.0, 0.0});
    aviator->bomberAttack(target);
    EXPECT_TRUE(aviator->getAircrafts().empty());
}

// Тест атаки вне радиуса действия
TEST_F(AviatorCruiserBomberTest, OutOfRangeBomberAttack) {
    acg::ship::coordinate target = {2000.0, 2000.0};
    aviator->setCurrentCoordinates({0.0, 0.0});

    acg::ship::airvector bombers = {{*bomber, {0.0, 0.0}}};
    aviator->modifyAircrafts(bombers);

    double initial_durability = bomber->getDurability();
    double initial_fuel = bomber->getFuelCapacity();

    aviator->bomberAttack(target);

    auto updated_aircraft = aviator->getAircrafts();
    EXPECT_EQ(updated_aircraft[0].first.getDurability(), initial_durability);
    EXPECT_EQ(updated_aircraft[0].first.getFuelCapacity(), initial_fuel);
}

// Тест атаки с неактивным бомбардировщиком
TEST_F(AviatorCruiserBomberTest, InactiveBomberAttack) {
    bomber->setActive(false);
    acg::ship::coordinate target = {100.0, 100.0};
    aviator->setCurrentCoordinates({0.0, 0.0});

    acg::ship::airvector bombers = {{*bomber, {0.0, 0.0}}};
    aviator->modifyAircrafts(bombers);

    double initial_durability = bomber->getDurability();
    double initial_fuel = bomber->getFuelCapacity();

    aviator->bomberAttack(target);

    auto updated_aircraft = aviator->getAircrafts();
    EXPECT_EQ(updated_aircraft[0].first.getDurability(), initial_durability);
    EXPECT_EQ(updated_aircraft[0].first.getFuelCapacity(), initial_fuel);
}