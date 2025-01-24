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
                900, true, 100, 50.0, 10.0, 10000.0, 20.0, 5000.0, 1000.0
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



class AircraftCarrierInterceptorTest : public ::testing::Test {
protected:
    void SetUp() override {
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 500.0
        );
        carrier->setMaxAircraftCapacity(5);

        // Создаем истребитель
        fighter = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER,
                100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 1000.0
        );
    }

    void TearDown() override {
        delete carrier;
        delete fighter;
    }

    acg::AircraftCarrier* carrier{};
    acg::Aircraft* fighter{};
};

// Тест атаки без истребителей
TEST_F(AircraftCarrierInterceptorTest, EmptyInterceptorAttack) {
    acg::ship::airvector enemy_aircraft = {
            {*fighter, {1000.0, 1000.0}}
    };
    carrier->interceptorAttack(enemy_aircraft);
    EXPECT_TRUE(carrier->getAircrafts().empty());
}

// Тест атаки с одним истребителем
TEST_F(AircraftCarrierInterceptorTest, SingleFighterAttack) {
    ASSERT_EQ(fighter->getFuelCapacity(), 1000.0); // Проверка начального значения
    fighter->setFuelCapacity(100000);
    acg::ship::airvector fighters = {
            {*fighter, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(fighters);

    acg::ship::airvector enemy_aircraft = {
            {*fighter, {500.0, 500.0}}
    };

    carrier->setCurrentCoordinates({0.0, 0.0});
    carrier->interceptorAttack(enemy_aircraft);

    auto updated_aircraft = carrier->getAircrafts();
    EXPECT_EQ(updated_aircraft.size(), 1);
    EXPECT_LT(updated_aircraft[0].first.getFuelCapacity(), 100000.0);
}

// Тест атаки с неактивным истребителем
TEST_F(AircraftCarrierInterceptorTest, InactiveFighterAttack) {
    fighter->setActive(false);
    acg::ship::airvector fighters = {
            {*fighter, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(fighters);

    acg::ship::airvector enemy_aircraft = {
            {*fighter, {500.0, 500.0}}
    };

    carrier->interceptorAttack(enemy_aircraft);

    auto updated_aircraft = carrier->getAircrafts();
    EXPECT_EQ(updated_aircraft[0].first.getFuelCapacity(), 1000.0);
}

// Тест атаки вне радиуса действия
TEST_F(AircraftCarrierInterceptorTest, OutOfRangeAttack) {
    acg::ship::airvector fighters = {
            {*fighter, {0.0, 0.0}}
    };
    carrier->modifyAircrafts(fighters);

    acg::ship::airvector enemy_aircraft = {
            {*fighter, {2000.0, 2000.0}}
    };

    carrier->interceptorAttack(enemy_aircraft);

    auto updated_aircraft = carrier->getAircrafts();
    EXPECT_EQ(updated_aircraft[0].first.getFuelCapacity(), 1000.0);
}



class AircraftCarrierCostTest : public ::testing::Test {
protected:
    void SetUp() override {
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 1000.0
        );

        aircraft = new acg::Aircraft(
                acg::Aircraft::AircraftType::FIGHTER,
                100, true, 100, 50.0, 10.0, 1000.0, 20.0, 500.0, 100.0
        );
    }

    void TearDown() override {
        delete carrier;
        delete aircraft;
    }

    acg::AircraftCarrier* carrier{};
    acg::Aircraft* aircraft{};
};

// Тест базовой стоимости
TEST_F(AircraftCarrierCostTest, BaseCost) {
    EXPECT_DOUBLE_EQ(carrier->calculateTotalCost(), 1000.0);
}

// Тест с самолетами
TEST_F(AircraftCarrierCostTest, CostWithAircraft) {
    carrier->setMaxAircraftCapacity(2);
    acg::ship::airvector aircrafts = {{*aircraft, {0.0, 0.0}}};
    carrier->modifyAircrafts(aircrafts);
    EXPECT_DOUBLE_EQ(carrier->calculateTotalCost(), 1500.0); // 1000 + 500
}

// Тест с повреждениями
TEST_F(AircraftCarrierCostTest, CostWithDamage) {
    carrier->setDurability(50); // 50% прочности
    EXPECT_DOUBLE_EQ(carrier->calculateTotalCost(), 500.0); // 1000 * 0.5
}

class AircraftCarrierDestinationTest : public ::testing::Test {
protected:
    void SetUp() override {
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 1000.0
        );
        carrier->setCurrentCoordinates({0.0, 0.0});
    }

    void TearDown() override {
        delete carrier;
    }

    acg::AircraftCarrier* carrier{};
};

// Тест установки близкой точки назначения
TEST_F(AircraftCarrierDestinationTest, NearDestination) {
    acg::ship::coordinate dest = {100.0, 100.0};
    carrier->setDestination(dest);
    auto final_dest = carrier->getDestinationCoordinates();
    EXPECT_EQ(final_dest.first, dest.first);
    EXPECT_EQ(final_dest.second, dest.second);
}

// Тест установки дальней точки назначения
TEST_F(AircraftCarrierDestinationTest, FarDestination) {
    carrier->setSpeed(10.0);
    acg::ship::coordinate dest = {1000.0, 1000.0};
    carrier->setDestination(dest);
    auto final_dest = carrier->getDestinationCoordinates();

    // Проверяем что точка назначения скорректирована
    EXPECT_LT(final_dest.first, dest.first);
    EXPECT_LT(final_dest.second, dest.second);
}





class AircraftCarrierMoveTest : public ::testing::Test {
protected:
    void SetUp() override {
        carrier = new acg::AircraftCarrier(
                acg::Ship::shiptype::AIRCRAFTCARRIER,
                "TestCarrier", "Captain", "John", 30.0, 100, 500.0
        );
    }

    void TearDown() override {
        delete carrier;
    }

    acg::AircraftCarrier* carrier{};
};

// Тест движения на расстояние больше скорости
TEST_F(AircraftCarrierMoveTest, MoveLongDistance) {
    carrier->setSpeed(10.0);
    carrier->setCurrentCoordinates({0.0, 0.0});
    carrier->setDestinationCoordinates({100.0, 100.0});

    carrier->move();

    auto pos = carrier->getCurrentCoordinates();
    EXPECT_GT(pos.first, 0.0);
    EXPECT_GT(pos.second, 0.0);
    EXPECT_LT(pos.first, 100.0);
    EXPECT_LT(pos.second, 100.0);
}

// Тест движения на короткое расстояние
TEST_F(AircraftCarrierMoveTest, MoveShortDistance) {
    carrier->setSpeed(50.0);
    carrier->setCurrentCoordinates({0.0, 0.0});
    carrier->setDestinationCoordinates({5.0, 5.0});

    carrier->move();

    auto pos = carrier->getCurrentCoordinates();
    EXPECT_DOUBLE_EQ(pos.first, 5.0);
    EXPECT_DOUBLE_EQ(pos.second, 5.0);
}

// Тест движения при нулевом расстоянии
TEST_F(AircraftCarrierMoveTest, MoveZeroDistance) {
    carrier->setSpeed(10.0);
    carrier->setCurrentCoordinates({5.0, 5.0});
    carrier->setDestinationCoordinates({5.0, 5.0});

    carrier->move();

    auto pos = carrier->getCurrentCoordinates();
    EXPECT_DOUBLE_EQ(pos.first, 5.0);
    EXPECT_DOUBLE_EQ(pos.second, 5.0);
}