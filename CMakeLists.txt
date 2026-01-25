#######################
#### Project Setup ####
#######################

## CMake Version Requirement ##
cmake_minimum_required(VERSION 3.16)

## Project Definition ##
project(2048 VERSION 1.0.0 LANGUAGES CXX)

## C++ Language Version ##
set(CMAKE_CXX_STANDARD 20)

## Require C++ Language Version ##
set(CMAKE_CXX_STANDARD_REQUIRED ON)




####################
#### Qt Package ####
####################

## Enable Qt MOC ##
set(CMAKE_AUTOMOC ON)

## Enable Qt UIC ##
set(CMAKE_AUTOUIC ON)

## Enable Qt RCC ##
set(CMAKE_AUTORCC ON)

## MinGW RC Compiler Setup for Windows RC File in Build ##
if(MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

## Get the Qt Package ##
option(QT_LOCAL_PATH "Path to Local Qt Compiler Installation" "")
if(QT_LOCAL_PATH)
    find_package(Qt6 REQUIRED COMPONENTS Widgets PATHS "${QT_LOCAL_PATH}" NO_DEFAULT_PATH)
else()
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
endif()




######################
#### Build Source ####
######################

## Add Header Files ##
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/ViewModel/2048App.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/GameConfig.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/2048Engine.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/2048UI.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/GameBoard.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/Scoreboard.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/NewGame.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Themes/Theme.hpp"
)

## Add Source Files ##
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/ViewModel/2048App.cpp"
    "${CMAKE_SOURCE_DIR}/src/Model/2048Engine.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/2048UI.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/GameBoard.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/Scoreboard.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/NewGame.cpp"
)

## Generate Binary Files in Bin Directory ##
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

## Generate OS-Specific 2048 Application Executable ##
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

## Target Include Files ##
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

## Link to Qt Framework ##
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)

## Output Binaries to Bin Directory ##
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe"
        COMMENT "Deploying Qt Dependencies to Output Directory..."
    )
endif()




###############################
#### Deploy to OS Platform ####
###############################

## Windows Application Build ##
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/windows/2048.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()

## macOS Application Build ##
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.connorsempf.2048"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE "2048.icns"
    )
    set_source_files_properties("${CMAKE_SOURCE_DIR}/resources/macOS/2048.icns"
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

## Linux Application Build ##
if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/2048.desktop" DESTINATION share/applications)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/2048.png" DESTINATION share/pixmaps)
endif()
