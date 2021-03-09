cmake_minimum_required(VERSION 3.6)
project(ttt)

set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        src/Main.cpp
        src/XMLReader.cpp
        src/XMLReader.h
        src/XMLReader.cpp
        src/XMLReader.h
        src/xml/tinystr.cpp
        src/xml/tinyxmlerror.cpp
        src/xml/tinystr.h
        src/xml/tinyxml.h
        src/xml/tinyxml.cpp
        src/xml/tinyxmlparser.cpp
        src/Exception.cpp
        src/Exception.h
        src/VaccinationCenter.cpp
        src/VaccinationCenter.h
        src/Hub.cpp
        src/Hub.h
        src/Simulation.cpp
        src/Simulation.h
        src/DesignByContract.h
        src/Utils.cpp
        src/Utils.h)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        src/MainTest.cpp
        src/Main.cpp
        src/XMLReader.cpp
        src/XMLReader.h
        src/XMLReader.cpp
        src/XMLReader.h
        src/xml/tinystr.cpp
        src/xml/tinyxmlerror.cpp
        src/xml/tinystr.h
        src/xml/tinyxml.h
        src/xml/tinyxml.cpp
        src/xml/tinyxmlparser.cpp
        src/Exception.cpp
        src/Exception.h
        src/VaccinationCenter.cpp
        src/VaccinationCenter.h
        src/Hub.cpp
        src/Hub.h
        src/Simulation.cpp
        src/Simulation.h
        src/DesignByContract.h
        src/Utils.cpp
        src/Utils.h)

# Create RELEASE target
add_executable(ttt ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(ttt_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(ttt_debug gtest)