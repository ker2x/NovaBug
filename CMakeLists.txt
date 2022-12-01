cmake_minimum_required(VERSION 3.10)


project(NovaBug VERSION 1.0.0 LANGUAGES CXX)

# Default to release builds
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)


#find_package(SFML REQUIRED COMPONENTS graphics system window)
find_package(Threads REQUIRED sfml-graphics sfml-system sfml-window sfml-main)

if(WIN32)
   set(WIN32_GUI WIN32)
endif()

set(SFML_LIBS sfml-graphics sfml-system sfml-window sfml-main)

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${WIN32_GUI} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})
