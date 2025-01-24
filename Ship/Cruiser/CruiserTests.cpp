#include <gtest/gtest.h>
#include "Cruiser.h"

class CruiserTest : public ::testing::Test {
protected:
    void SetUp() override {
        cruiser = new acg::Cruiser(
                acg::Ship::shiptype::CRUISER,
                "TestCruiser", "Captain", "John",
                30.0, 100, 500.0, 5, 1000
        );

        // Создаем тестовое оружие
        weapon = new acg::Armament("TestWeapon",
                                   acg::Armament::ArmamentType::LIGHT,
                                   100, 1000.0, 60.0, 100, 2.0, 200.0);
        weapon->setActive(true);
        weapon->setCurrentAmmo(100);

        // Добавляем оружие крейсеру
        acg::ship::armvector weapons = {*weapon};
        cruiser->modifyArmament(weapons);
    }

    void TearDown() override {
        delete cruiser;
        delete weapon;
    }

    acg::Cruiser* cruiser{};
    acg::Armament* weapon{};
};


TEST_F(CruiserTest, CalculateAvailableAmmoStorage) {
    // Создаем тестовые боеприпасы
    acg::ship::ammomap ammo = {
            {"TestAmmo", {500, 10.0, 4}},
            {"TestAmmo2", {300, 15.0, 5}}
    };

    cruiser->modifyAmmoInfo(ammo);
    int available = cruiser->calculateAvailableAmmoStorage();

    EXPECT_EQ(available, 200); // 1000 - (500 + 300)
}

TEST_F(CruiserTest, FireAtShip) {
    acg::ship::coordinate target = {100.0, 100.0};
    cruiser->setCurrentCoordinates({0.0, 0.0});
    int initial_ammo = weapon->getCurrentAmmo();
    weapon->setRateOfFire(1);
    weapon->setRangeOfFire(1000);
    weapon->setReloadSpeed(20);

    // Увеличиваем количество итераций для гарантированного срабатывания интервала
    for(int i = 0; i < 20; i++) {
        cruiser->fireAtShip(target);
        if(i % 4 == 3) {  // Проверяем каждый 4-й выстрел
            EXPECT_LT(cruiser->getArmament()[0].getCurrentAmmo(), initial_ammo);
            initial_ammo = weapon->getCurrentAmmo();
        }
    }
}

TEST_F(CruiserTest, ReloadWeapon) {
    // Устанавливаем начальные условия
    weapon->setCurrentAmmo(0);
    acg::ship::ammomap ammo = {
            {weapon->getAmmoName(), {100, 10.0, 25}}
    };
    cruiser->modifyAmmoInfo(ammo);

    // Тестируем перезарядку
    for(int i = 0; i < static_cast<int>(weapon->getReloadSpeed()) + 1; i++) {
        cruiser->reloadWeapon(*weapon);
    }

    EXPECT_GT(weapon->getCurrentAmmo(), 0);
    EXPECT_LT(cruiser->getAmmoInfo(weapon->getAmmoName()).quantity, 100);
}

class CruiserTest2 : public ::testing::Test {
protected:
    void SetUp() override {
        cruiser = new acg::Cruiser(
                acg::Ship::shiptype::CRUISER,
                "TestCruiser",
                "Captain",
                "John",
                30.0, // speed
                100,  // durability
                1000.0, // cost
                5,    // max_armament_capacity
                1000  // storage_capacity
        );

        // Создаем тестовое вооружение
        weapon = new acg::Armament(
                "TestWeapon",
                acg::Armament::ArmamentType::LIGHT,
                100, // damage
                1000.0, // range
                60.0,  // rate of fire
                100,   // max ammo
                2.0,   // reload speed
                200.0  // cost
        );
    }

    void TearDown() override {
        delete cruiser;
        delete weapon;
    }

    acg::Cruiser* cruiser{};
    acg::Armament* weapon{};
};

// Тесты для calculateTotalCost
TEST_F(CruiserTest2, CalculateTotalCostEmpty) {
    EXPECT_DOUBLE_EQ(cruiser->calculateTotalCost(), 1000.0); // Только базовая стоимость
}

TEST_F(CruiserTest2, CalculateTotalCostWithWeapons) {
    acg::ship::armvector weapons = {*weapon};
    cruiser->modifyArmament(weapons);

    double expected = 1000.0 + 200.0; // Базовая + стоимость оружия
    EXPECT_DOUBLE_EQ(cruiser->calculateTotalCost(), expected);
}

TEST_F(CruiserTest2, CalculateTotalCostWithDamage) {
    cruiser->setDurability(50); // 50% прочности
    double expected = 1000.0 * 0.5;
    EXPECT_DOUBLE_EQ(cruiser->calculateTotalCost(), expected);
}

// Тесты для setDestination
TEST_F(CruiserTest2, SetDestinationIntervalCheck) {
    acg::ship::coordinate dest = {100.0, 100.0};

    // 5-й вызов должен изменить координаты
    cruiser->setDestination(dest);
    auto final_dest = cruiser->getDestinationCoordinates();
    EXPECT_NE(final_dest.first, 0.0);
    EXPECT_NE(final_dest.second, 0.0);
}

// Тесты для move
TEST_F(CruiserTest2, MoveIntervalCheck) {
    acg::ship::coordinate dest = {100.0, 100.0};
    cruiser->setDestinationCoordinates(dest);

    auto initial_pos = cruiser->getCurrentCoordinates();
    cruiser->move();
    auto final_pos = cruiser->getCurrentCoordinates();
    EXPECT_NE(final_pos.first, initial_pos.first);
    EXPECT_NE(final_pos.second, initial_pos.second);
}




class CruiserFireTest : public ::testing::Test {
protected:
    void SetUp() override {
        cruiser = new acg::Cruiser(
                acg::Ship::shiptype::CRUISER,
                "TestCruiser",
                "Captain",
                "John",
                30.0,
                100,
                500.0,
                5,    // max_armament_capacity
                1000  // storage_capacity
        );

        weapon = new acg::Armament(
                "TestWeapon",
                acg::Armament::ArmamentType::LIGHT,
                100,   // damage
                1000.0, // range
                60.0,  // rate of fire
                100,   // max ammo
                2.0,   // reload speed
                200.0  // cost
        );

        weapon->setActive(true);
        weapon->setCurrentAmmo(100);

        acg::ship::armvector weapons = {*weapon};
        cruiser->modifyArmament(weapons);
    }

    void TearDown() override {
        delete cruiser;
        delete weapon;
    }

    acg::Cruiser* cruiser{};
    acg::Armament* weapon{};
};

// Тест стрельбы по самолетам в пределах досягаемости
TEST_F(CruiserFireTest, FireAtAircraftInRange) {
    acg::ship::airvector aircraft = {
            {acg::Aircraft(
                    acg::Aircraft::AircraftType::FIGHTER,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 100.0
            ), {500.0, 500.0}}
    };

    cruiser->setCurrentCoordinates({510.0, 510.0});
    int initial_ammo = weapon->getCurrentAmmo();
    // Делаем несколько выстрелов
    for(int i = 0; i < 8; i++) {
        cruiser->fireAtAircraft(aircraft);
    }

    EXPECT_LT(cruiser->getArmament()[0].getCurrentAmmo(), initial_ammo);
}

// Тест стрельбы по самолетам вне зоны досягаемости
TEST_F(CruiserFireTest, FireAtAircraftOutOfRange) {
    acg::ship::airvector aircraft = {
            {acg::Aircraft(
                    acg::Aircraft::AircraftType::FIGHTER,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 100.0
            ), {2000.0, 2000.0}}
    };

    cruiser->setCurrentCoordinates({0.0, 0.0});
    int initial_ammo = weapon->getCurrentAmmo();

    cruiser->fireAtAircraft(aircraft);

    EXPECT_EQ(weapon->getCurrentAmmo(), initial_ammo);
}




class CruiserFireTest2 : public ::testing::Test {
protected:
    void SetUp() override {
        cruiser = new acg::Cruiser(
                acg::Ship::shiptype::CRUISER,
                "TestCruiser",
                "Captain",
                "John",
                30.0,
                100,
                500.0,
                5,    // max_armament_capacity
                1000  // storage_capacity
        );

        weapon = new acg::Armament(
                "TestWeapon",
                acg::Armament::ArmamentType::LIGHT,
                100,   // damage
                1000.0, // range
                60.0,  // rate of fire
                100,   // max ammo
                2.0,   // reload speed
                200.0  // cost
        );

        weapon->setActive(true);
        weapon->setCurrentAmmo(100);

        acg::ship::armvector weapons = {*weapon};
        cruiser->modifyArmament(weapons);
    }

    void TearDown() override {
        delete cruiser;
        delete weapon;
    }

    acg::Cruiser* cruiser{};
    acg::Armament* weapon{};
};

// Тест интервала между выстрелами
TEST_F(CruiserFireTest2, FireIntervalCheck) {
    acg::ship::airvector aircraft = {
            {acg::Aircraft(
                    acg::Aircraft::AircraftType::FIGHTER,
                    100, true, 100, 50.0, 10.0, 1000.0, 20.0, 5000.0, 100.0
            ), {500.0, 500.0}}
    };

    cruiser->setCurrentCoordinates({0.0, 0.0});
    int initial_ammo = weapon->getCurrentAmmo();

    // Второй вызов должен произвести выстрел
    cruiser->fireAtAircraft(aircraft);
    EXPECT_LT(cruiser->getArmament()[0].getCurrentAmmo(), initial_ammo);
}