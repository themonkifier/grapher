cmake_minimum_required(VERSION 3.5)

project(workspace VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(GENERAL_SOURCES
        src/calculatorline.cpp
        src/calculatorline.h
        src/matrixline.cpp
        src/matrixline.h
)

set(CALCULATOR_SOURCES
        src/calculator/ast.cpp
        src/calculator/ast.h
        src/calculator/calculator.cpp
        src/calculator/calculator.h
        src/calculator/lexer.cpp
        src/calculator/lexer.h
        src/calculator/parser.cpp
        src/calculator/parser.h
)

set(UTIL_SOURCES
        src/calculator/util/context.cpp
        src/calculator/util/context.h
        src/calculator/util/errors.h
        src/calculator/util/functions.h
        src/calculator/util/functions.cpp
        src/calculator/util/numerical.h
        src/calculator/util/numerical.cpp
)
set(MATRIX_SOURCES
        src/calculator/matrix/matrix.cpp
        src/calculator/matrix/matrix.h
)

set(PROJECT_SOURCES
        ${GENERAL_SOURCES}
        ${CALCULATOR_SOURCES}
        ${UTIL_SOURCES}
        ${MATRIX_SOURCES}
        main.cpp
        grapher.cpp
        grapher.h
        grapher.ui
        inputhandler.cpp
        inputhandler.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(workspace
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET workspace APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(workspace SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(workspace
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(workspace PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.workspace)
endif()
set_target_properties(workspace PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS workspace
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(workspace)
endif()
