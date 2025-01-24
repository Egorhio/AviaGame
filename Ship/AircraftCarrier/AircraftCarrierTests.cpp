#include <gtest/gtest.h>
#include "AircraftCarrier.h"

class AircraftCarrierBomberTest : public ::testing::Test {
protected:
    void SetUp() override {
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 500.0
        );
        carrier->setMaxAircraftCapacity(100);

        // Создаем тестовые бомбардировщики
        bomber1 = new acg::Aircraft(
                acg::Aircraft::AircraftType::ATTACK,
                100, true, 100, 50.0, 10.0, 10000.0, 20.0, 5000.0, 1000.0
        );

        bomber2 = new acg::Aircraft(
                acg::Aircraft::AircraftType::ATTACK,
                100, true, 50, 50.0, 10.0, 10000.0, 20.0, 5000.0, 1000.0
        );

        bomber3 = new acg::Aircraft(
                acg::Aircraft::AircraftType::ATTACK,
                100, true, 80, 50.0, 10.0, 10000.0, 20.0, 5000.0, 1000.0
        );
    }

    void TearDown() override {
        delete carrier;
        delete bomber1;
        delete bomber2;
        delete bomber3;
    }

    acg::AircraftCarrier* carrier{};
    acg::Aircraft *bomber1{}, *bomber2{}, *bomber3{};
};

// Тест атаки без самолетов
TEST_F(AircraftCarrierBomberTest, EmptyBomberAttack) {
    acg::ship::coordinate target = {1000.0, 1000.0};
    carrier->setCurrentCoordinates({0.0, 0.0});
    carrier->bomberAttack(target);
    // Проверяем что ничего не произошло
    EXPECT_TRUE(carrier->getAircrafts().empty());
}

// Тест атаки с одной волной бомбардировщиков
TEST_F(AircraftCarrierBomberTest, SingleWaveBomberAttack) {
    acg::ship::coordinate target = {100.0, 100.0};
    carrier->setCurrentCoordinates({0.0, 0.0});

    // Добавляем 3 бомбардировщика (одна волна)
    acg::ship::airvector bombers = {
            {*bomber1, {0.0, 0.0}},
            {*bomber2, {0.0, 0.0}},
            {*bomber3, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(bombers);

    // Выполняем атаку
    carrier->bomberAttack(target);

    // Проверяем что самолеты получили урон и потратили топливо
    auto updated_aircraft = carrier->getAircrafts();
    for(const auto& [aircraft, _] : updated_aircraft) {
        EXPECT_LT(aircraft.getDurability(), 100);
        EXPECT_LT(aircraft.getFuelCapacity(), 10000.0);
    }
}

// Тест атаки с несколькими волнами
TEST_F(AircraftCarrierBomberTest, MultiWaveBomberAttack) {
    acg::ship::coordinate target = {100.0, 100.0};
    carrier->setCurrentCoordinates({0.0, 0.0});

    // Добавляем 6 бомбардировщиков (две волны)
    acg::ship::airvector bombers;
    for(int i = 0; i < 6; i++) {
        bombers.push_back({*bomber1, {0.0, 0.0}});
    }
    carrier->modifyAircrafts(bombers);

    carrier->bomberAttack(target);

    // Проверяем что вторая волна нанесла меньше урона
    auto updated_aircraft = carrier->getAircrafts();
    std::vector<int> damages;
    for(const auto& [aircraft, _] : updated_aircraft) {
        damages.push_back(aircraft.getDamage());
    }

    for(size_t i = 3; i < damages.size(); i++) {
        EXPECT_LT(damages[i], damages[i-3]); // Урон второй волны меньше
    }
}

// Тест атаки вне радиуса действия
TEST_F(AircraftCarrierBomberTest, OutOfRangeBomberAttack) {
    acg::ship::coordinate target = {2000.0, 2000.0};
    carrier->setCurrentCoordinates({0.0, 0.0});

    acg::ship::airvector bombers = {
            {*bomber1, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(bombers);

    carrier->bomberAttack(target);

    // Проверяем что характеристики не изменились
    auto updated_aircraft = carrier->getAircrafts();
    EXPECT_EQ(updated_aircraft[0].first.getDurability(), bomber1->getDurability());
    EXPECT_EQ(updated_aircraft[0].first.getFuelCapacity(), bomber1->getFuelCapacity());
}