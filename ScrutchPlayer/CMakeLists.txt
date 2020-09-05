cmake_minimum_required(VERSION 3.17)
project(ScrutchPlayer)

#C++
set(CMAKE_CXX_STANDARD 17)

#General
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#Qt
set(CMAKE_PREFIX_PATH "${QTDIR}\\msvc2017_64")
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

# TagLib
find_library(TAGLIB tag "F:\\Work\\C++\\taglib\\out\\install\\x64-Release\\lib")
include_directories(SYSTEM "F:\\Work\\C++\\taglib\\out\\install\\x64-Release\\include")

#Project
include_directories(includes)

set(LOCAL_SRCS
        main.cpp
        src/MainWindow.cpp
        src/LibraryParser.cpp
        src/Track.cpp
        )

set(LOCAL_INCLUDES
        includes/MainWindow.h
        includes/LibraryParser.h
        includes/Track.h
        )

add_executable(ScrutchPlayer ${LOCAL_SRCS} ${LOCAL_INCLUDES})

target_link_libraries(ScrutchPlayer Qt5::Widgets)
target_link_libraries(ScrutchPlayer ${TAGLIB} )
