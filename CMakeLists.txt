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

# Get the Qt plugins directory
get_target_property(QT_PLUGINS_DIR Qt6::Core QT_PLUGIN_DIR)
if(NOT QT_PLUGINS_DIR)
    # Fallback: set manually or from environment
    set(QT_PLUGINS_DIR "$ENV{QT6_INSTALL_PLUGINS_DIR}")
endif()

qt_standard_project_setup()

# ✅ Define sources
set(SOURCES
    src/base/tableschema.cpp
    src/tables/categorytable.cpp
    src/tables/statetable.cpp
    src/tables/vendortable.cpp
    src/main.cpp
    src/databasemanager.cpp
    src/databasetables.cpp
    src/state.cpp
    src/tableaccess.cpp
    src/tablemodel.cpp
)

set(HEADERS
    src/base/columnconstraint.h
    src/base/tablemixin.h
    src/base/tableschema.h
    src/tables/categorytable.h
    src/tables/statetable.h
    src/tables/vendortable.h
    src/databasemanager.h
    src/databasetables.h
    src/state.h
    src/tableaccess.h
    src/tablemodel.h
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

# ✅ Installation and deployment rules
include(GNUInstallDirs)

install(TARGETS pFinance
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET pFinance
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# ✅ Copy Qt SQL drivers to build output (fixes missing SQL drivers)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${QT_PLUGINS_DIR}/sqldrivers"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers"
)

# Copy PostgreSQL client DLLs (adjust the path as needed)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libpq.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libpq.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libiconv-2.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libiconv-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libintl-9.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libintl-9.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libssl-3-x64.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libssl-3-x64.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libcrypto-3-x64.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libcrypto-3-x64.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/bin/PostgreSQL/17/bin/libwinpthread-1.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libwinpthread-1.dll"
)