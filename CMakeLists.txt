cmake_minimum_required(VERSION 3.16)
project(pFinance LANGUAGES CXX)

# ✅ Enable Qt's MOC system for QObject/meta-types
set(CMAKE_AUTOMOC ON)

# ✅ Find required Qt 6 modules
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Sql
)

# ✅ Define your main executable and link sources
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/accessbase.cpp
    src/databasemanager.cpp
    src/dataregistry.cpp
    src/vendoraccess.cpp
    src/vendormodel.cpp
)

# Optional: Group headers for IDE visibility
target_sources(${PROJECT_NAME}
    PRIVATE
    src/accessbase.h
    src/databasemanager.h
    src/dataregistry.h
    src/vendoraccess.h
    src/vendormodel.h
)

# ✅ Include source directory for headers
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# ✅ Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
)

# ✅ Set C++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# ✅ Define your QML import path (where qmldir and Main.qml reside)
set(QML_DIR "${CMAKE_SOURCE_DIR}/qml/pFinance")
set_property(TARGET ${PROJECT_NAME} PROPERTY QT_QML_IMPORT_PATH "${QML_DIR}")


# ✅ Copy QML files to runtime folder
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml/pFinance)

file(GLOB_RECURSE QML_FILES
    "${CMAKE_SOURCE_DIR}/qml/pFinance/*.qml"
)

foreach(file IN LISTS QML_FILES)
    file(RELATIVE_PATH relPath "${CMAKE_SOURCE_DIR}/qml/pFinance" "${file}")
    configure_file(${file} "${CMAKE_BINARY_DIR}/qml/pFinance/${relPath}" COPYONLY)
endforeach()

# ✅ Copy qmldir file for runtime loading
configure_file(${QML_DIR}/qmldir ${CMAKE_BINARY_DIR}/qml/pFinance/qmldir COPYONLY)
