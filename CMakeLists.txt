cmake_minimum_required(VERSION 3.23)
project(3)

set(CMAKE_CXX_STANDARD 23)

include_directories(Armament Aircraft Mission Ship Table-template)

add_executable(3
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
        Global.cpp
        Global.h
        Ship/Ship-Interface.h
        Ship/Cruiser/Cruiser.cpp
        Ship/Cruiser/Cruiser.h
        Ship/AviatorCruiser/AviatorCruiser.cpp
        Ship/AviatorCruiser/AviatorCruiser.h
        Ship/AircraftCarrier/AircraftCarrier.cpp
        Ship/AircraftCarrier/AircraftCarrier.h
        Ship/AircraftCarrier/AircraftCarrier-Interface.h
        Ship/Cruiser/Cruiser-Interface.h
        main.cpp
        Table-template/Table.inl
        Table-template/Table.inl)

add_executable(project
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
        Global.cpp
        Global.h
        Ship/Ship-Interface.h
        Ship/Cruiser/Cruiser.cpp
        Ship/Cruiser/Cruiser.h
        Ship/AviatorCruiser/AviatorCruiser.cpp
        Ship/AviatorCruiser/AviatorCruiser.h
        Ship/AircraftCarrier/AircraftCarrier.cpp
        Ship/AircraftCarrier/AircraftCarrier.h
        Ship/AircraftCarrier/AircraftCarrier-Interface.h
        Ship/Cruiser/Cruiser-Interface.h
        main.cpp
        Table-template/Table.inl
        Table-template/Table.inl
        ../../SFML-2.6.2/include/SFML/Graphics.hpp
        ../../SFML-2.6.2/include/SFML/Window.hpp
        ../../SFML-2.6.2
        ../../SFML-2.6.2/include/SFML)
