cmake_minimum_required(VERSION 3.23)
project(3)

set(CMAKE_CXX_STANDARD 23)

include_directories(Armament Aircraft Mission Ship Table)

add_executable(3
        Armament/Armament.cpp
        Armament/Armament.h
        Aircraft/Aircraft.cpp
        Aircraft/Aircraft.h
        Ship/Ship.cpp
        Ship/Ship.h
        Mission/Mission.cpp
        Mission/Mission.h
        Table/Table.cpp
        Table/Table.h
        Global.cpp
        Global.h
        Ship/Ship-Interface.h
        Ship/Cruiser/Cruiser.cpp
        Ship/Cruiser/Cruiser.h
        Ship/AviatorCruiser/AviatorCruiser.cpp
        Ship/AviatorCruiser/AviatorCruiser.h
        Ship/Aircraft/AircraftCarrier.cpp
        Ship/Aircraft/AircraftCarrier.h
        Ship/Aircraft/AircraftCarrier-Interface.h
        Ship/Cruiser/Cruiser-Interface.h
        main.cpp)
