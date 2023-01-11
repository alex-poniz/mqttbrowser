#github
#user alex-poniz
#token  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
cmake_minimum_required(VERSION 3.14)

project(test1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

find_library(MOSQUITTO_LIBRARY mosquitto HINTS "C:/Program Files/mosquitto/devel")
include_directories(test1 PRIVATE "C:/Program Files/mosquitto/devel")
#link_directories(test1 PRIVATE "C:/Program Files/mosquitto/devel")

set(PROJECT_SOURCES
        main.cpp
        appcontroller.cpp
        appcontroller.h
        appdata.h
        appdata.cpp
        qml.qrc
        mqtt/mqttclient.cpp
        mqtt/mqttclient.h
        mqtt/mqttcommunicator.h
        mqtt/mqttcommunicator.cpp
        mqtt/mqttlistener.h
        mqtt/mqttlistener.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET test1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(test1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(test1
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(test1
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick ${MOSQUITTO_LIBRARY})

set_target_properties(test1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS test1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(test1)
    qt_finalize_executable(test1)
endif()
