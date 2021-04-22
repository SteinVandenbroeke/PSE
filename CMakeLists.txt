cmake_minimum_required(VERSION 3.6)
project(VaccinDistributor)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED on)
#set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(src/gtest/include)

# Set Library dir
link_directories(src/gtest/lib)

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
        src/Vaccin.cpp
        src/Vaccin.h
        src/Hub.cpp
        src/Hub.h
        src/Simulation.cpp
        src/Simulation.h
        src/DesignByContract.h
        src/Utils.cpp
        src/Utils.h
        src/Vaccin.cpp
        src/Vaccin.h)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        src/VdTests.cpp
        src/VdDomainTests.cpp
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
        src/Vaccin.cpp
        src/Vaccin.h
        src/Hub.cpp
        src/Hub.h
        src/Simulation.cpp
        src/Simulation.h
        src/DesignByContract.h
        src/Utils.cpp
        src/Utils.h
        src/VdUtilsTests.cpp
        src/VdSimulateTests.cpp
        src/VdXMLReaderTests.cpp
        src/Vaccin.cpp
        src/Vaccin.h)

# Create RELEASE target
add_executable(VaccinDistributor ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(VaccinDistributor_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(VaccinDistributor_debug gtest)