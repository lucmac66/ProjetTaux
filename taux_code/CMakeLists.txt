cmake_minimum_required(VERSION 3.0)
project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Add jlparser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_subdirectory(src/jlparser)

# Find nlohmann_json to parse json files
# Add nlohmann_json::nlohmann_json to target_link_libraries
find_package(nlohmann_json REQUIRED)

set(fxhedging_hdrs  src/json_reader.hpp)
add_library(fx_hedging src/json_reader.cpp ${fxhedging_hdrs})
target_link_libraries(fx_hedging ${PNL_LIBRARIES} nlohmann_json::nlohmann_json)


### Ne rien modifier dans la partie au dessus de cette ligne.

# Test the parser
add_executable(parser-example src/parser_example.cpp)
target_link_libraries(parser-example jl_parser ${LIBS} nlohmann_json::nlohmann_json)

add_executable(mc-example src/mc_example.cpp src/FixedRebalancing.cpp src/GridRebalancing.cpp src/Portfolio.cpp src/CallQuanto.cpp src/Currency.cpp src/RiskyAsset.cpp src/CallCurrency.cpp src/QuantoExchange.cpp src/ForeignAsian.cpp src/json_reader.cpp src/MonteCarlo.cpp src/BlackScholesModel.cpp src/Hedger.cpp src/ForeignPerfBasket.cpp src/Position.cpp)
target_link_libraries(mc-example jl_parser ${LIBS} nlohmann_json::nlohmann_json)

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
