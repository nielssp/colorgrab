project(colorgrab)

cmake_minimum_required(VERSION 2.8.11)

find_package(wxWidgets COMPONENTS core base xml xrc adv REQUIRED)

include(${wxWidgets_USE_FILE})

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
