#include <iostream>
#include <Table.h>
#include <Ship.h>

int main() {
    using namespace acg;

    // Создаем таблицу для хранения объектов Ship
    ShipTable<Ship> table;

    // Добавляем корабли в таблицу
    table.addShip("Alpha01", new Ship("Alpha Explorer", 5000.0));
    table.addShip("Bravo02", new Ship("Bravo Carrier", 7500.5));
    table.addShip("Charlie03", new Ship("Charlie Scout", 2000.3));

    // Извлекаем корабль по позывному
    Ship* retrievedShip = table.getShip("Bravo02");
    table.removeShip("Alpha01");
    std::cout << "count : " << table.getShipCount() << std::endl;
    // Удаляем корабль
    if (!table.getShip("Alpha01")) {
        std::cout << "Alpha01 successfully removed." << std::endl;
    }

    // Выводим все оставшиеся корабли с помощью итератора
    auto iterator = table.getIterator();
    while (iterator.hasNext()) {
        auto [callSign, ship] = iterator.get();
        std::cout << "Call sign: " << callSign << ", Ship: " << ship->getName() << std::endl;
        iterator.next();
    }


    return 0;
}