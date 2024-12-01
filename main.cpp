#include <iostream>
#include <Table.h>

int main() {
    using namespace acg;

    // Создаем таблицу для хранения объектов Ship
    ShipTable<Ship> table;

    // Добавляем корабли в таблицу
    table.addShip("Alpha01", new Ship("Alpha Explorer", 5000.0));
    table.addShip("Bravo02", new Ship("Bravo Carrier", 7500.5));
    table.addShip("Charlie03", new Ship("Charlie Scout", 2000.3));

    // Извлекаем корабль по позывному
    // Ship* retrievedShip = table.getShip("Bravo02");

    // Удаляем корабль
    table.removeShip("Alpha01");
    if (!table.getShip("Alpha01")) {
        std::cout << "Alpha01 successfully removed." << std::endl;
    }

    // Выводим все оставшиеся корабли с помощью итератора
    auto iterator = table.getIterator();
    std::cout << "Remaining ships in the table:" << std::endl;
    while (iterator.hasNext()) {
        auto [callSign, ship] = iterator.get();
        std::cout << "Call Sign: " << callSign << " -> ";
        iterator.next();
    }

    return 0;
}