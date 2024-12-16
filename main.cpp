////#include <iostream>
////#include <Table.h>
////#include <Ship.h>
////
////int main() {
////    using namespace acg;
////
////    // Создаем таблицу для хранения объектов Ship
////    ShipTable<Ship> table;
////
////    // Добавляем корабли в таблицу
////    table.addShip("Alpha01", new Ship);
////
////    // Извлекаем корабль по позывному
////    Ship* retrievedShip = table.getShip("Bravo02");
////    table.removeShip("Alpha01");
////    std::cout << "count : " << table.getShipCount() << std::endl;
////    // Удаляем корабль
////    if (!table.getShip("Alpha01")) {
////        std::cout << "Alpha01 successfully removed." << std::endl;
////    }
////
////    // Выводим все оставшиеся корабли с помощью итератора
////    auto iterator = table.getIterator();
////    while (iterator.hasNext()) {
////        auto [callSign, ship] = iterator.get();
////        std::cout << "Call sign: " << callSign << ", Ship: " << ship->getName() << std::endl;
////        iterator.next();
////    }
////
////
////    return 0;
////}
//
//#include "../../SFML-2.6.2/include/SFML/Graphics.hpp"
//#include "../../SFML-2.6.2/include/SFML/Window.hpp"
//#include <iostream>
//
//// Функция проверки, был ли клик внутри кнопки
//bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) {
//    sf::FloatRect bounds = button.getGlobalBounds();
//    return bounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
//}
//
//int main() {
//    // Создаем окно 800x600
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Интерфейс SFML");
//
//    // **Кнопка "Переместить"**
//    sf::RectangleShape button(sf::Vector2f(200, 50)); // Размер кнопки
//    button.setFillColor(sf::Color::Green);           // Зеленый цвет кнопки
//    button.setPosition(50, 50);                      // Позиция кнопки
//
//    sf::Font font;
//    if (!font.loadFromFile("arial.ttf")) {
//        std::cerr << "Не удалось загрузить шрифт" << std::endl;
//        return -1;
//    }
//    sf::Text buttonText("Переместить", font, 20);  // Текст кнопки
//    buttonText.setFillColor(sf::Color::White);     // Белый цвет текста
//    buttonText.setPosition(button.getPosition().x + 20, button.getPosition().y + 10);
//
//    // **Объект корабля**
//    sf::RectangleShape ship(sf::Vector2f(50, 50)); // Прямоугольный "кораблик"
//    ship.setFillColor(sf::Color::Cyan);
//    ship.setPosition(400, 300); // Центр окна
//
//    // Главный цикл
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            // Обрабатываем клик мышью
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) { // ЛКМ
//                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
//                    if (isMouseOverButton(button, mousePosition)) {
//                        // Кнопка "Переместить" была нажата -> сдвигаем корабль
//                        ship.move(50, 0); // Двигаем кораблик вправо
//                    }
//                }
//            }
//
//            // Обрабатываем события клавиатуры для перемещения корабля
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//                ship.move(-5, 0); // Влево
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//                ship.move(5, 0); // Вправо
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//                ship.move(0, -5); // Вверх
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//                ship.move(0, 5); // Вниз
//            }
//
//        }
//
//        // Обновляем отображение
//        window.clear(sf::Color::Black);   // Очищаем окно
//        window.draw(button);              // Рисуем кнопку
//        window.draw(buttonText);          // Рисуем текст кнопки
//        window.draw(ship);                // Рисуем корабль
//        window.display();                 // Отображаем содержимое окна
//    }
//
//    return 0;
//}
//
