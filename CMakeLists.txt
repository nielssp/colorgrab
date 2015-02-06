project(colorgrab)

cmake_minimum_required(VERSION 2.8.11)

find_package(wxWidgets COMPONENTS core base xml xrc adv REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(src)

file(GLOB_RECURSE SRC_LIST src/*.cpp)

if(WIN32)
  set(wxWidgets_USE_STATIC true)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
  set(SRC_LIST ${SRC_LIST} src/win_resources.rc)
  set(wxWidgets_CONFIG_OPTIONS --static=yes)
  add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
else()
  add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()


target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
