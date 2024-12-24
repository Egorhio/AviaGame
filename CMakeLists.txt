cmake_minimum_required(VERSION 3.23)
project(3)

set(CMAKE_CXX_STANDARD 23)

include_directories(Armament Aircraft Mission Ship Table-template)

set(SOURCES
        Armament/Armament.cpp
        Armament/Armament.h
        Aircraft/Aircraft.cpp
        Aircraft/Aircraft.h
        Ship/Ship.cpp
        Ship/Ship.h
        Mission/Mission.cpp
        Mission/Mission.h
        Table-template/Table.inl
        Table-template/Table.h
        Ship/Ship-Interface.h
        Ship/Cruiser/Cruiser.cpp
        Ship/Cruiser/Cruiser.h
        Ship/AviatorCruiser/AviatorCruiser.cpp
        Ship/AviatorCruiser/AviatorCruiser.h
        Ship/AircraftCarrier/AircraftCarrier.cpp
        Ship/AircraftCarrier/AircraftCarrier.h
        Ship/AircraftCarrier/AircraftCarrier-Interface.h
        Ship/Cruiser/Cruiser-Interface.h
)

add_executable(3 ${SOURCES} main.cpp
        Ship/AviatorCruiser/AviatorCruiser-Interface.h)

add_executable(project
        ${SOURCES}
        main.cpp
        ../../SFML-2.6.2/include/SFML/Graphics.hpp
        ../../SFML-2.6.2/include/SFML/Window.hpp
        ../../SFML-2.6.2
        ../../SFML-2.6.2/include/SFML)


# Для работы с FetchContent
include(FetchContent)

# Загрузка GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# Необязательно: Ускорение компиляции тестов на Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Создаем тесты 1
add_executable(Misiiontests
        ${SOURCES}
        Mission/MissionTests.cpp
)

# Создаем тесты 2
add_executable(Aircrafttests
        Aircraft/Aircraft.cpp
        Aircraft/Aircraft.h
        Aircraft/AircraftTests.cpp
)

add_executable(Armamenttests
        Armament/Armament.cpp
        Armament/Armament.h
        Armament/ArmamentTests.cpp
)

add_executable(Tabletests
        ${SOURCES}
        Table-template/TableTests.cpp
)

# Привязываем GoogleTest к тестовым исполняемым файлам
target_link_libraries(Misiiontests gtest gtest_main)
target_link_libraries(Aircrafttests gtest gtest_main)
target_link_libraries(Armamenttests gtest gtest_main)
target_link_libraries(Tabletests gtest gtest_main)

# Регистрируем тесты
add_test(NAME all_tests COMMAND tests)

add_executable(MeasuringTimeRaid
        ${SOURCES}
        Mission/MeasuringSpeedWorkMethod.cpp
)

