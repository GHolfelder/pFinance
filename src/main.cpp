#include "databasemanager.h"
#include "dataregistry.h"
#include "vendortable.h"
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

    // Create data registry
    DataRegistry registry(dbManager.database(), &app);

    // Set context properties in QML
    /// VendorModel *vendorModel = new VendorModel(dbManager.database(), &app);

    VendorTable *vendorTable = registry.vendortable();
    TableAccess *vendorAccess = new TableAccess(dbManager.database(), vendorTable, &app);
    TableModel *vendorModel = new TableModel(dbManager.database(), vendorTable, &app);
    engine.rootContext()->setContextProperty("vendorAccess", vendorAccess);
    engine.rootContext()->setContextProperty("vendorModel", vendorModel);

    // Load main application window
    engine.addImportPath("qml");
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
