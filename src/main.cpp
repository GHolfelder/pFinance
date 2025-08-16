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
    TableAccess *vendorAccess = new TableAccess(dbManager.database(), &tables, "Vendors", &app);
    TableModel *vendorModel = new TableModel(dbManager.database(), &tables, "Vendors", &app);
    engine.rootContext()->setContextProperty("vendorAccess", vendorAccess);
    engine.rootContext()->setContextProperty("vendorModel", vendorModel);

    // Load main application window
    engine.addImportPath("qml");
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
