cmake_minimum_required(VERSION 3.23)
project(3)

set(CMAKE_CXX_STANDARD 23)

include_directories(Armament Aircraft Mission Ship Table-template Game ../1)

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
        Ship/AviatorCruiser/AviatorCruiser-Interface.h
)

set(ADDING
        Game/BattlefieldView.cpp
        Game/BattlefieldView.h
        Game/CombatView.cpp
        Game/CombatView.h
        Game/MissionView.cpp
        Game/MissionView.h
        Game/ShipView.cpp
        Game/ShipView.h
        AI/ai.cpp
        AI/ai.h
)

set(ADDITIONAL
        ../1/additional.h
        ../1/additional.cpp
)

add_executable(main-game ${SOURCES} ${ADDING} ${ADDITIONAL}
        main.cpp
        Game/main-game.h
)

add_executable(Game ${SOURCES} ${ADDING} ${ADDITIONAL}
        Game/main-game.cpp
        Game/main-game.h
)

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

add_executable(Shiptests
        ${SOURCES}
        Ship/ShipTests.cpp
)

add_executable(Cruisertests
        ${SOURCES}
        Ship/Cruiser/CruiserTests.cpp
)

add_executable(AircraftCarriertests
        ${SOURCES}
        Ship/AircraftCarrier/AircraftCarrierTests.cpp
)

add_executable(AviatorCruisertests
        ${SOURCES}
        Ship/AviatorCruiser/AviatorCruiserTests.cpp
)

# Привязываем GoogleTest к тестовым исполняемым файлам
target_link_libraries(Misiiontests gtest gtest_main)
target_link_libraries(Aircrafttests gtest gtest_main)
target_link_libraries(Armamenttests gtest gtest_main)
target_link_libraries(Tabletests gtest gtest_main)
target_link_libraries(Shiptests gtest gtest_main)
target_link_libraries(Cruisertests gtest gtest_main)
target_link_libraries(AircraftCarriertests gtest gtest_main)
target_link_libraries(AviatorCruisertests gtest gtest_main)


# Регистрируем тесты
add_test(NAME all_tests COMMAND tests)

add_executable(MeasuringTimeRaid
        ${SOURCES}
        Mission/MeasuringSpeedWorkMethod.cpp
)

# Добавление nlohmann/json
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Добавьте в target_link_libraries для ваших исполняемых файлов
target_link_libraries(main-game PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Game PRIVATE nlohmann_json::nlohmann_json)




