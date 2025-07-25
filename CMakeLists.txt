cmake_minimum_required(VERSION 3.16)
project(pFinance LANGUAGES CXX)

# ✅ Qt recommended setup macro
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtSetup.cmake OPTIONAL)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QmlModuleHelpers.cmake OPTIONAL)

# ✅ Enable Qt features
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Sql
)

qt_standard_project_setup()

# ✅ Define sources
set(SOURCES
    src/main.cpp
    src/accessbase.cpp
    src/databasemanager.cpp
    src/dataregistry.cpp
    src/vendoraccess.cpp
    src/vendormodel.cpp
    src/modelbase.cpp
)

set(HEADERS
    src/accessbase.h
    src/databasemanager.h
    src/dataregistry.h
    src/vendoraccess.h
    src/vendormodel.h
    src/modelbase.h
)

# ✅ Add main executable
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# ✅ Link Qt modules
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
)

# ✅ Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ✅ Set C++ standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# ✅ Setup QML import path and copy resources
set(QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml/pFinance")
set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_QML_IMPORT_PATH "${QML_DIR}"
    QT_QML_SOURCE_DIR "${QML_DIR}"
)

# ✅ Copy QML files
file(GLOB_RECURSE QML_FILES "${QML_DIR}/*.qml" "${QML_DIR}/qmldir")
foreach(file IN LISTS QML_FILES)
    file(RELATIVE_PATH relPath "${QML_DIR}" "${file}")
    configure_file(${file} "${CMAKE_BINARY_DIR}/qml/pFinance/${relPath}" COPYONLY)
endforeach()
