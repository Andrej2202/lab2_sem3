cmake_minimum_required(VERSION 3.16)
project(StudentIndexSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/base
     ${CMAKE_SOURCE_DIR}/gui
)

add_executable(student_system_gui
    src/main_gui.cpp
    src/Student.cpp
    src/IndexSystem.cpp
    gui/MainWindow.cpp
    gui/MainWindow.hpp          
    gui/StudentTableModel.cpp
    gui/StudentTableModel.hpp   
    gui/BenchmarkDialog.cpp
    gui/BenchmarkDialog.hpp     
    gui/BenchmarkTableModel.cpp
    gui/BenchmarkTableModel.hpp 
)

target_link_libraries(student_system_gui PRIVATE Qt6::Widgets)

if(MSVC)
    target_compile_options(student_system_gui PRIVATE /W4)
else()
    target_compile_options(student_system_gui PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(test_system
    tests/test_main.cpp
    tests/test_student.cpp
    tests/test_pair.cpp
    tests/test_sorted_sequence.cpp
    tests/test_binary_tree.cpp
    tests/test_file_storage.cpp
    tests/test_index_system.cpp
    src/Student.cpp
    src/IndexSystem.cpp
)

if(MSVC)
    target_compile_options(test_system PRIVATE /W4)
else()
    target_compile_options(test_system PRIVATE -Wall -Wextra -pedantic)
endif()