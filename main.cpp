//#include "Mission.h"
//#include <thread>
//#include <mutex>
//
//using namespace std;
//using namespace acg;
//
//int main() {
//
//    setlocale(LC_ALL, "ru");
//
//    auto* mission = new Mission("Commander", 5, 1000.0);
//
//    // Создаем тестовый корабль-авианосец
//    auto* carrier = new AircraftCarrier(
//            Ship::shiptype::AIRCRAFTCARRIER,
//            "TestCarrier", "Captain", "John", 30.0, 100, 500.0
//    );
//
//
//
//
//    cout << carrier->getMaxAircraftCapacity() << endl;
//
//    thread th([&]() {
//        carrier->setMaxAircraftCapacity(5);
//    });
//
//    for (int i = 0; i < 5; ++i) {
//        this_thread::sleep_for(chrono::milliseconds(500));
//        cout << this_thread::get_id() << endl;
//    }
//
//    th.join();
//
//    cout << carrier->getMaxAircraftCapacity() << endl;
//
//    thread del([&] () {
//        mission->destroyShip("CARRIER1");
//        delete carrier;
//        delete mission;
//    });
//
//    del.join();
//
//    return 0;
//}

#include "Mission.h"
#include <iostream>

int main() {
    // Создаем крейсер
    auto* cruiser = new acg::Cruiser(
            acg::Ship::shiptype::CRUISER,
            "TestCruiser", "Captain", "John",
            30.0, 100, 500.0, 5, 1000
    );

    // Создаем и добавляем оружие
    auto* weapon = new acg::Armament(
            "TestWeapon",
            acg::Armament::ArmamentType::LIGHT,
            100, // урон
            1000.0, // дальность стрельбы
            60.0, // скорострельность
            100, // макс. боезапас
            2.0, // скорость перезарядки
            200.0 // стоимость
    );
    weapon->setActive(true);
    weapon->setRateOfFire(1);

    // Добавляем оружие крейсеру
    acg::ship::armvector weapons = {*weapon};
    cruiser->modifyArmament(weapons);

    // Устанавливаем координаты крейсера и цели
    cruiser->setCurrentCoordinates({0.0, 0.0});
    acg::ship::coordinate target = {500.0, 500.0};

    // Стреляем по цели и выводим результаты
    std::cout << "Initial ammo: " << cruiser->getArmament()[0].getCurrentAmmo() << std::endl;
    cruiser->fireAtShip(target);
    cruiser->fireAtShip(target);
    cruiser->fireAtShip(target);
    cruiser->fireAtShip(target);
    std::cout << "Remaining ammo: " << cruiser->getArmament()[0].getCurrentAmmo() << std::endl;

    delete cruiser;
    delete weapon;
    return 0;
}
