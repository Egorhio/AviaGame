#include <gtest/gtest.h>
#include "Table.h"
#include "Cruiser/Cruiser.h"


class ShipTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table = new acg::ShipTable<acg::Ship>();
    }

    void TearDown() override {
        delete table;
    }

    acg::ShipTable<acg::Ship>* table{};
};

// Тест конструктора и начального состояния
TEST_F(ShipTableTest, Constructor) {
    EXPECT_EQ(table->getShipCount(), 0);
    EXPECT_TRUE(table->empty());
}

// Тест добавления корабля
TEST_F(ShipTableTest, AddShip) {
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER,
                               "Test", "Captain", "John", 30.0, 100, 500.0);

    table->addShip("TEST1", ship);
    EXPECT_EQ(table->getShipCount(), 1);
    EXPECT_FALSE(table->empty());
    EXPECT_EQ(table->getShip("TEST1"), ship);
}

// Тест удаления корабля
TEST_F(ShipTableTest, RemoveShip) {
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER,
                               "Test", "Captain", "John", 30.0, 100, 500.0);

    table->addShip("TEST1", ship);
    table->removeShip("TEST1");
    EXPECT_EQ(table->getShipCount(), 0);
    EXPECT_TRUE(table->empty());
    EXPECT_EQ(table->getShip("TEST1"), nullptr);
}

// Тест очистки таблицы
TEST_F(ShipTableTest, Clear) {
    for(int i = 0; i < 5; i++) {
        auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                   "Test", "Captain", "John", 30.0, 100, 500.0);
        table->addShip("TEST" + std::to_string(i), ship);
    }

    table->clear();
    EXPECT_EQ(table->getShipCount(), 0);
    EXPECT_TRUE(table->empty());
}

// Тест итератора
TEST_F(ShipTableTest, Iterator) {
    auto* ship1 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test1", "Captain", "John", 30.0, 100, 500.0);
    auto* ship2 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test2", "Captain", "Jack", 30.0, 100, 500.0);

    table->addShip("TEST1", ship1);
    table->addShip("TEST2", ship2);

    auto iter = table->getIterator();
    int count = 0;
    while(iter.hasNext()) {
        auto [call_sign, ship] = iter.get();
        EXPECT_TRUE(ship != nullptr);
        count++;
        iter.next();
    }
    EXPECT_EQ(count, 2);
}

// Тест на дублирование ключей
TEST_F(ShipTableTest, DuplicateKeys) {
    auto* ship1 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test", "Captain", "John", 30.0, 100, 500.0);
    auto* ship2 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test", "Captain", "Jack", 30.0, 100, 500.0);

    table->addShip("TEST1", ship1);
    table->addShip("TEST1", ship2);
    EXPECT_EQ(table->getShipCount(), 1);
}

// В TableTests.cpp
TEST_F(ShipTableTest, ForwardIteratorTest) {
    static_assert(std::forward_iterator<acg::ShipTable<acg::Ship>::Iterator>);

    auto iter = table->getIterator();
    std::forward_iterator auto iter2 = iter; // Должно компилироваться, если это итератор прямого доступа

    // Тест на многократное прохождение
    auto first = iter;
    auto second = iter;
    EXPECT_EQ(first, second);

    // Тест на инкремент
    ++first;
    EXPECT_NE(first, second);
}