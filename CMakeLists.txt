cmake_minimum_required(VERSION 3.5)

project(pmlib-demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)
include_directories(.)

add_library(pmlib STATIC
        pmlib/process_image.cpp
        pmlib/enum_rotate.cpp
        pmlib/enum_funcs.cpp
        pmlib/getset.cpp
        pmlib/mapSS2RS.cpp
        pmlib/edge_link.cpp
        pmlib/isolate_channel.cpp
        pmlib/quad_diff_amp.cpp
        pmlib/region_growing.cpp
        pmlib/moore_boundary.cpp
        pmlib/moore_neighborhood.cpp
        pmlib/point_in_image.cpp
        pmlib/edge_extract_strongest.cpp
        pmlib/edge_extract_thr_first.cpp
        pmlib/edge_extract_thr_strongest.cpp
        pmlib/add_roi_offset.cpp
)

set(PROJECT_SOURCES
        main.cpp
        dialog.cpp
        dialog.h
        dialog.ui
        mapInit.cpp
        cameraThread.cpp
        clickableLabel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pmlib-demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pmlib-demo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(pmlib-demo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(pmlib-demo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(pmlib PRIVATE ${OpenCV_LIBS})
target_link_libraries(pmlib-demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS} pmlib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pmlib-demo)
endif()
set_target_properties(pmlib-demo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS pmlib-demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pmlib-demo)
endif()
