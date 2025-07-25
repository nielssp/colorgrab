cmake_minimum_required(VERSION 3.10)

project(colorgrab LANGUAGES CXX)


find_package(wxWidgets COMPONENTS core base xml xrc adv REQUIRED)

include(GNUInstallDirs)

include(${wxWidgets_USE_FILE})

file(GLOB_RECURSE SRC_LIST src/*.cpp)

if(WIN32)
  set(wxWidgets_USE_STATIC true)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
  set(SRC_LIST ${SRC_LIST} src/win_resources.rc)
  set(wxWidgets_CONFIG_OPTIONS --static=yes)
  add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
else()
  add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(FILES pkg/arch/${PROJECT_NAME}.desktop DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/applications)
install(FILES img/scalable.svg DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/icons/hicolor/scalable/apps RENAME ${PROJECT_NAME}.svg)
foreach(res 16 32 48 64 128 256)
  install(FILES img/${res}x${res}.png DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/icons/hicolor/${res}x${res}/apps RENAME ${PROJECT_NAME}.png)
endforeach()
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR})
install(FILES README.md DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
