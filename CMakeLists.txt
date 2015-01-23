cmake_minimum_required(VERSION 2.8.11)

project(cgrabtest2)



include_directories(
    D:/Home/Dropbox/Projects/cgrabtest2/
    D:/Home/Dropbox/Projects/cgrabtest2/

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
    " -g -O0 -Wall wx-config Error: wxWidgets hasnt been found installed at D:/Program Files (x86).

Please use the --prefix flag (as in wx-config --prefix=C:/wxWidgets)
or set the environment variable WXWIN (as in WXWIN=C:/wxWidgets)
to specify where is your installation of wxWidgets.")

add_executable(cgrabtest2 ${CXX_SRCS} ${C_SRCS})

