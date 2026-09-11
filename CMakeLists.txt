cmake_minimum_required(VERSION 3.15)
project(ElysiumEngineProject VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection for compiler definitions
if(WIN32)
  add_compile_definitions(ELYSIUM_PLATFORM_WINDOWS)
elseif(APPLE)
  add_compile_definitions(ELYSIUM_PLATFORM_MAC)
elseif(UNIX)
  add_compile_definitions(ELYSIUM_PLATFORM_LINUX)
endif()

# Raylib via FetchContent
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Add engine subdirectory
add_subdirectory(engine)

# Add application
add_executable(ElysiumApp app/main.cpp)

target_link_libraries(ElysiumApp PRIVATE ElysiumEngine raylib)

# Set include directories for the app to find engine headers
target_include_directories(ElysiumApp PRIVATE
    engine/core/include
    engine/physics/include
)

# Add Performance Tests
add_executable(PhysicsPerformanceTest test/PhysicsPerformanceTest.cpp)
target_link_libraries(PhysicsPerformanceTest PRIVATE ElysiumEngine)
target_include_directories(PhysicsPerformanceTest PRIVATE
    engine/core/include
    engine/physics/include
)

add_executable(BroadPhasePerformanceTest test/BroadPhasePerformanceTest.cpp)
target_link_libraries(BroadPhasePerformanceTest PRIVATE ElysiumEngine)
target_include_directories(BroadPhasePerformanceTest PRIVATE
    engine/core/include
    engine/physics/include
)

add_executable(BoundaryTest test/BoundaryTest.cpp)
target_link_libraries(BoundaryTest PRIVATE ElysiumEngine)
target_include_directories(BoundaryTest PRIVATE
    engine/core/include
    engine/physics/include
)

add_executable(FrictionTest test/FrictionTest.cpp)
target_link_libraries(FrictionTest PRIVATE ElysiumEngine)
target_include_directories(FrictionTest PRIVATE
    engine/core/include
    engine/physics/include
)

enable_testing()
add_test(NAME PhysicsPerformanceTest COMMAND PhysicsPerformanceTest)
add_test(NAME BroadPhasePerformanceTest COMMAND BroadPhasePerformanceTest)
add_test(NAME BoundaryTest COMMAND BoundaryTest)
add_test(NAME FrictionTest COMMAND FrictionTest)
