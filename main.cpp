#include "databasemanager.h"
#include "dataregistry.h"
#include "vendoraccess.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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
#ifdef QT_DEBUG
    VendorAccess* vendor = registry.vendoraccess();
    if (vendor->count() == 0)
        vendor->generateSample(20);
    else
        qDebug() << "Vendors" << vendor->count();
#endif

    // Load main application window
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
