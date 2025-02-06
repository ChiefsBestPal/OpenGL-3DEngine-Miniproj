cmake_minimum_required(VERSION 3.29)
project(a1)

set(CMAKE_CXX_STANDARD 20)

add_executable(a1 main.cpp Part1.cpp
        Part1.h
        Part2.cpp
        Part2.h
)
if(UNIX)
    add_custom_target(run_in_terminal
            COMMAND gnome-terminal -- bash -c "./a1; exec bash"
            DEPENDS a1
            COMMENT "Running a1 in terminal"
    )
elseif(WIN32)
    # For Windows, use 'start' to run in cmd
    add_custom_target(run_in_terminal
            COMMAND cmd /c start "" a1
            DEPENDS a1
            COMMENT "Running a1 in terminal"
    )
endif()