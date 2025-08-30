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

    // Create data tables
    DatabaseTables tables(&app);

    // Connect and initialize data
    DatabaseManager dbManager(&app);
    if (!dbManager.connect())
        return -1;
    if (!dbManager.initializeSchema(&tables))
        return -1;

    // Set context properties in QML
    TableAccess *categoryAccess = new TableAccess(dbManager.database(), &tables, "Categories", &app);
    TableModel *categoryModel = new TableModel(dbManager.database(), &tables, "Categories", &app);
    TableAccess *vendorAccess = new TableAccess(dbManager.database(), &tables, "Vendors", &app);
    TableModel *vendorModel = new TableModel(dbManager.database(), &tables, "Vendors", &app);
    engine.rootContext()->setContextProperty("vendorAccess", vendorAccess);
    engine.rootContext()->setContextProperty("vendorModel", vendorModel);
    engine.rootContext()->setContextProperty("categoryAccess", categoryAccess);
    engine.rootContext()->setContextProperty("categoryModel", categoryModel);

    // Load main application window
    engine.addImportPath("qml");
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
