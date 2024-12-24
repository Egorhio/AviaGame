#include <gtest/gtest.h>
#include <thread>
#include "Armament.h"

class ArmamentTest : public ::testing::Test {
protected:
    void SetUp() override {
        armament = new acg::Armament();
    }

    void TearDown() override {
        delete armament;
    }

    acg::Armament* armament{};
};

// Тесты для установки и валидации имени
TEST_F(ArmamentTest, NameValidation) {
    EXPECT_NO_THROW(armament->setName("Cannon"));
    EXPECT_EQ(armament->getName(), "Cannon");
    EXPECT_THROW(armament->setName(""), std::invalid_argument);
}

// Тесты для типа оружия
TEST_F(ArmamentTest, TypeValidation) {
    EXPECT_NO_THROW(armament->setType(acg::Armament::ArmamentType::LIGHT));
    EXPECT_EQ(armament->getType(), acg::Armament::ArmamentType::LIGHT);
    EXPECT_NO_THROW(armament->setType(acg::Armament::ArmamentType::HEAVY));
}

// Тесты для боеприпасов
TEST_F(ArmamentTest, AmmoOperations) {
    EXPECT_NO_THROW(armament->setMaxAmmoCapacity(100));
    EXPECT_NO_THROW(armament->setCurrentAmmo(50));
    EXPECT_EQ(armament->getCurrentAmmo(), 50);
    EXPECT_THROW(armament->setCurrentAmmo(-1), std::invalid_argument);
    EXPECT_THROW(armament->setCurrentAmmo(101), std::invalid_argument);
}

// Тесты для характеристик оружия
TEST_F(ArmamentTest, WeaponCharacteristics) {
    EXPECT_NO_THROW(armament->setDamage(100));
    EXPECT_NO_THROW(armament->setRangeOfFire(1000.0));
    EXPECT_NO_THROW(armament->setRateOfFire(60.0));
    EXPECT_THROW(armament->setDamage(-1), std::invalid_argument);
    EXPECT_THROW(armament->setRangeOfFire(0.0), std::invalid_argument);
    EXPECT_THROW(armament->setRateOfFire(0.0), std::invalid_argument);
}

// Тест стрельбы
TEST_F(ArmamentTest, ShootingMechanism) {
    armament->setActive(true);
    armament->setMaxAmmoCapacity(10);
    armament->setCurrentAmmo(10);
    armament->setRateOfFire(1.0);
    armament->shoot();
    EXPECT_EQ(armament->getCurrentAmmo(), 9);
    // Тест скорострельности
    armament->shoot(); // Не должен стрелять из-за задержки
    EXPECT_EQ(armament->getCurrentAmmo(), 9);
}

// Тест перезарядки
TEST_F(ArmamentTest, ReloadingMechanism) {
    armament->setActive(true);
    armament->setMaxAmmoCapacity(10);
    armament->setCurrentAmmo(5);
    armament->setReloadSpeed(1.0);

    armament->reload();
    armament->reload();
    armament->reload();
    EXPECT_EQ(armament->getCurrentAmmo(), 10);
}
