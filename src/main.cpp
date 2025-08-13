#include "databasemanager.h"
#include "databasetables.h"
#include "tableaccess.h"
#include "tablemodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Connect and initialize data
    DatabaseManager dbManager(&app);
    if (!dbManager.connect())
        return -1;
    if (!dbManager.initializeSchema())
        return -1;

    // Create data tables
    DatabaseTables tables(&app);

    // Set context properties in QML
    /// VendorModel *vendorModel = new VendorModel(dbManager.database(), &app);

    TableSchema *stateTable = tables.fetch("States");
    TableSchema *vendorTable = tables.fetch("Vendors");
    TableAccess *vendorAccess = new TableAccess(dbManager.database(), vendorTable, &app);
    TableModel *vendorModel = new TableModel(dbManager.database(), vendorTable, &app);
    engine.rootContext()->setContextProperty("vendorAccess", vendorAccess);
    engine.rootContext()->setContextProperty("vendorModel", vendorModel);

    // Load main application window
    engine.addImportPath("qml");
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
