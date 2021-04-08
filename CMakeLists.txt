cmake_minimum_required(VERSION 3.5)

project(PowerControl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-mwindows")        #去掉控制台
SET(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS}/level='requireAdministrator'/uiAccess='false'")  #以管理员权限运行



# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        singleapplication.h
        singleapplication.cpp
        mainwindow.ui
        images.qrc
        icon.rc

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PowerControl ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(PowerControl SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PowerControl
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(PowerControl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
find_package(Qt6 COMPONENTS Network REQUIRED)
target_link_libraries(PowerControl PRIVATE Qt6::Network)
find_package(Qt6 COMPONENTS Gui REQUIRED)
target_link_libraries(PowerControl PRIVATE Qt6::Gui)
find_package(Qt6 COMPONENTS Core REQUIRED)
target_link_libraries(PowerControl PRIVATE Qt6::Core)
find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
 target_link_libraries(PowerControl PUBLIC Qt6::Core5Compat)




