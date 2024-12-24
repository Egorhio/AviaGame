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

TEST_F(ShipTableTest, ForwardIteratorTest1) {
    // Добавим данные в таблицу
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER,
                               "Test", "Captain", "John", 30.0, 100, 500.0);
    table->addShip("TEST1", ship);

    static_assert(std::forward_iterator<acg::ShipTable<acg::Ship>::Iterator>);
    auto iter = table->getIterator();
    std::forward_iterator auto iter2 = iter;

    EXPECT_EQ(iter, iter2);
    ++iter;
    EXPECT_NE(iter, iter2);
}


// Тест на корректность работы операторов сравнения
TEST_F(ShipTableTest, IteratorComparison) {
    auto* ship1 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test1", "Captain", "John", 30.0, 100, 500.0);
    auto* ship2 = new acg::Ship(acg::Ship::shiptype::CRUISER,
                                "Test2", "Captain", "Jack", 30.0, 100, 500.0);

    table->addShip("TEST1", ship1);
    table->addShip("TEST2", ship2);

    auto iter1 = table->getIterator();
    auto iter2 = table->getIterator();

    EXPECT_TRUE(iter1 == iter2);
    iter1.next();
    EXPECT_FALSE(iter1 == iter2);
}

// Тест на инкремент итератора
TEST_F(ShipTableTest, IteratorIncrement) {
    auto* ship = new acg::Ship(acg::Ship::shiptype::CRUISER,
                               "Test", "Captain", "John", 30.0, 100, 500.0);
    table->addShip("TEST1", ship);

    auto iter = table->getIterator();
    auto [call_sign, _] = iter.get();
    EXPECT_EQ(call_sign, "TEST1");

    ++iter;
    EXPECT_FALSE(iter.hasNext());
}

// Тест на работу с пустой таблицей
TEST_F(ShipTableTest, EmptyTableIterator) {
    auto iter = table->getIterator();
    EXPECT_FALSE(iter.hasNext());
    EXPECT_THROW(iter.get(), std::out_of_range);
}

// Тест для проверки, что Iterator соответствует концепту std::forward_iterator
TEST_F(ShipTableTest, IteratorConceptCheck) {
    static_assert(std::forward_iterator<acg::ShipTable<acg::Ship>::Iterator>,
                  "Iterator должен соответствовать концепту std::forward_iterator");
}

TEST_F(ShipTableTest, IteratorConcepts) {
    // Проверка соответствия концепту std::forward_iterator
    static_assert(std::forward_iterator<acg::ShipTable<acg::Ship>::Iterator>,
                  "Iterator должен соответствовать концепту std::forward_iterator");

    // Проверка соответствия конкретным типам
    using Iterator = acg::ShipTable<acg::Ship>::Iterator;

    // Проверка iterator_category
    static_assert(std::is_same_v<Iterator::iterator_category, std::forward_iterator_tag>,
                  "iterator_category должен быть std::forward_iterator_tag");

    // Проверка value_type
    static_assert(std::is_same_v<Iterator::value_type, std::pair<std::string, acg::Ship*>>,
                  "value_type должен быть std::pair<std::string, acg::Ship*>");

    // Проверка difference_type
    static_assert(std::is_same_v<Iterator::difference_type, std::ptrdiff_t>,
                  "difference_type должен быть std::ptrdiff_t");

    // Проверка pointer
    static_assert(std::is_same_v<Iterator::pointer, std::pair<std::string, acg::Ship*>*>,
                  "pointer должен быть std::pair<std::string, acg::Ship*>*");

    // Проверка reference
    static_assert(std::is_same_v<Iterator::reference, std::pair<std::string, acg::Ship*>&>,
                  "reference должен быть std::pair<std::string, acg::Ship*>&");
}