cmake_minimum_required(VERSION 2.8.11)

project(ColorGrab)



include_directories(
    /home/darx/Workspace/ColorGrab/
    /home/darx/Workspace/ColorGrab/
    /usr/lib/wx/include/gtk2-unicode-3.0
    /usr/include/wx-3.0

)

add_definitions(
    -D_FILE_OFFSET_BITS=64
    -DWXUSINGDLL
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  $(shell wx-config --libs --debug)")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    main.cpp
    MainFrame.cpp
    wxcrafter.cpp
    wxcrafter_bitmaps.cpp
    ImagePanel.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -Wall -pthread")

add_executable(ColorGrab ${CXX_SRCS} ${C_SRCS})

