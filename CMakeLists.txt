cmake_minimum_required(VERSION 3.23)
project(3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ThreadSanitizer нужен для проверки многопоточного налёта, но доступен только
# на Linux/macOS (Clang/GCC). Включается явно: -DLAB3_ENABLE_TSAN=ON
option(LAB3_ENABLE_TSAN "Собрать с ThreadSanitizer (только Clang/GCC на Linux/macOS)" OFF)
if (LAB3_ENABLE_TSAN)
    if (MSVC OR MINGW OR WIN32)
        message(WARNING "LAB3_ENABLE_TSAN проигнорирован: ThreadSanitizer не поддерживается на Windows")
    else ()
        add_compile_options(-g -fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif ()
endif ()

# На MinGW линкуем рантайм статически: .exe не зависит от libstdc++/libgcc/libwinpthread
# в PATH (иначе может подхватиться несовместимая DLL из Git for Windows и уронить программу).
if (MINGW AND NOT LAB3_ENABLE_TSAN)
    add_link_options(-static -static-libgcc -static-libstdc++)
endif ()

option(LAB3_BUILD_TESTS "Собирать модульные тесты (нужен GoogleTest)" ON)

include_directories(Armament Aircraft Mission Ship Table-template Game common)

# ---------------------------------------------------------------------------
#  Внешние зависимости
#
#  Скачиваются автоматически (FetchContent). Если сеть перехватывает TLS
#  («certificate signer not trusted») — вариант а) добавить -DCMAKE_TLS_VERIFY=OFF
#  (архивы всё равно проверяются по SHA256 ниже);  вариант б) заранее скачать
#  архивы браузером и указать -DFETCHCONTENT_SOURCE_DIR_JSON=... /
#  -DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=...
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

if (LAB3_BUILD_TESTS)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
            URL_HASH SHA256=f179ec217f9b3b3f3c6e8b02d3e7eda997b49e4ce26d6b235c9053bec9c0bf9f
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # Ускорение компиляции тестов на Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
endif ()

# ---------------------------------------------------------------------------
#  Ядро модели (общий код для игры, тестов и бенчмарка)
# ---------------------------------------------------------------------------
set(CORE_SOURCES
        Armament/Armament.cpp
        Aircraft/Aircraft.cpp
        Ship/Ship.cpp
        Mission/Mission.cpp
        Ship/Cruiser/Cruiser.cpp
        Ship/AviatorCruiser/AviatorCruiser.cpp
        Ship/AircraftCarrier/AircraftCarrier.cpp
        common/additional.cpp
)

add_library(lab3_core STATIC ${CORE_SOURCES})
target_include_directories(lab3_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Armament
        ${CMAKE_CURRENT_SOURCE_DIR}/Aircraft
        ${CMAKE_CURRENT_SOURCE_DIR}/Mission
        ${CMAKE_CURRENT_SOURCE_DIR}/Ship
        ${CMAKE_CURRENT_SOURCE_DIR}/Ship/Cruiser
        ${CMAKE_CURRENT_SOURCE_DIR}/Ship/AviatorCruiser
        ${CMAKE_CURRENT_SOURCE_DIR}/Ship/AircraftCarrier
        ${CMAKE_CURRENT_SOURCE_DIR}/Table-template
        ${CMAKE_CURRENT_SOURCE_DIR}/Game
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)
target_link_libraries(lab3_core PUBLIC nlohmann_json::nlohmann_json)

# ---------------------------------------------------------------------------
#  Игра
# ---------------------------------------------------------------------------
add_executable(Game
        Game/BattlefieldView.cpp
        Game/MissionView.cpp
        Game/ShipView.cpp
        AI/ai.cpp
        Game/main-game.cpp
        main.cpp
)
target_link_libraries(Game PRIVATE lab3_core)

# ---------------------------------------------------------------------------
#  Бенчмарк однопоточного/многопоточного налёта
# ---------------------------------------------------------------------------
add_executable(MeasuringTimeRaid Mission/MeasuringSpeedWorkMethod.cpp)
target_link_libraries(MeasuringTimeRaid PRIVATE lab3_core)

# ---------------------------------------------------------------------------
#  Тесты (только если LAB3_BUILD_TESTS=ON)
# ---------------------------------------------------------------------------
if (LAB3_BUILD_TESTS)
    # Юнит-тесты, не зависящие от ядра целиком
    add_executable(Aircrafttests Aircraft/Aircraft.cpp Aircraft/AircraftTests.cpp)
    target_link_libraries(Aircrafttests PRIVATE gtest gtest_main)

    add_executable(Armamenttests Armament/Armament.cpp Armament/ArmamentTests.cpp)
    target_link_libraries(Armamenttests PRIVATE gtest gtest_main)

    # Тесты, использующие полную модель
    add_executable(Missiontests          Mission/MissionTests.cpp)
    add_executable(Tabletests            Table-template/TableTests.cpp)
    add_executable(Shiptests             Ship/ShipTests.cpp)
    add_executable(Cruisertests          Ship/Cruiser/CruiserTests.cpp)
    add_executable(AircraftCarriertests  Ship/AircraftCarrier/AircraftCarrierTests.cpp)
    add_executable(AviatorCruisertests   Ship/AviatorCruiser/AviatorCruiserTests.cpp)

    foreach (t Missiontests Tabletests Shiptests Cruisertests AircraftCarriertests AviatorCruisertests)
        target_link_libraries(${t} PRIVATE lab3_core gtest gtest_main)
    endforeach ()

    foreach (t Aircrafttests Armamenttests Missiontests Tabletests Shiptests
              Cruisertests AircraftCarriertests AviatorCruisertests)
        add_test(NAME ${t} COMMAND ${t})
    endforeach ()
endif ()
