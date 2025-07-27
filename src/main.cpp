#include "databasemanager.h"
#include "dataregistry.h"
#include "vendoraccess.h"
#include "vendormodel.h"

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
#ifdef QT_DEBUG
    VendorAccess* vendor = registry.vendoraccess();
    if (vendor->count() == 0)
        vendor->generateSample(20);
    else
        qDebug() << "Vendor count:" << vendor->count();
#endif

    // Set context properties in QML
    VendorModel *vendorModel = new VendorModel(dbManager.database(), &app);
    engine.rootContext()->setContextProperty("vendorModel", vendorModel);
    engine.rootContext()->setContextProperty("vendorAccess", vendor);

    // Load main application window
    engine.addImportPath("qml");
    engine.loadFromModule("pFinance", "Main");

    return app.exec();
}
