#include "vendoraccess.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QUuid>
#include <QDebug>

const QStringList VendorAccess::COLUMN_NAMES = {
    "id",
    "name",
    "address1",
    "address2",
    "city",
    "state",
    "postal_code",
    "phone",
    "unpaid_balance"
};

/**
 * @brief Vendor access constructor
 *
 * @param db Database object where tables are located
 * @param parent Reference to parent class.
 */
VendorAccess::VendorAccess(QSqlDatabase db, QObject *parent) : AccessBase(db, parent) {
    setObjectName("VendorAccess");
}

/**
 * @brief Count number of records in vendor table
 *
 * Note: No signals are emitted from this method
 *
 * @return Number of records in table, -1 if an error occurred
 */
int VendorAccess::count() const {
    QSqlQuery query(m_db);

    if (!query.exec("SELECT COUNT(*) FROM vendors")) {
        return -1;
    }
    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

/**
 * @brief Add new vendor to database
 *
 * @param vendorData Variant map containing fields and their associated value
 * @return True if successful, otherwise false
 */
bool VendorAccess::add(const QVariantMap &vendorData) {
    QSqlQuery query(m_db);
    QString guid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    query.prepare(R"(
        INSERT INTO vendors (
            id, name, address1, address2, city, state, postal_code, phone, unpaid_balance
        ) VALUES (
            :id, :name, :address1, :address2, :city, :state, :postal_code, :phone, :unpaid_balance
        )
    )");

    query.bindValue(":id", guid);
    query.bindValue(":name", vendorData["name"]);
    query.bindValue(":address1", vendorData["address1"]);
    query.bindValue(":address2", vendorData["address2"]);
    query.bindValue(":city", vendorData["city"]);
    query.bindValue(":state", vendorData["state"]);
    query.bindValue(":postal_code", vendorData["postal_code"]);
    query.bindValue(":phone", vendorData["phone"]);
    query.bindValue(":unpaid_balance", vendorData["unpaid_balance"]);

    if (!query.exec())
        return fail(query.lastError().text());

    return success("Vendor added with ID:", guid);
}

/**
 * @brief Retrieve vendor from database
 *
 * The caller is responsible for clearing the variant map if desired.
 *
 * @param id Id of vendor to be retrieved
 * @param result Variant map where fields and their associated value will be returned
 * @return Variant mapo containing retrieved data
 */
QVariantMap VendorAccess::get(const QString &id) {
    QSqlQuery query(m_db);
    QVariantMap result;

    // Initialize empty record when no id is provided
    if (id == "") {
        for (const QString &name : COLUMN_NAMES) {
            if (name == "unpaid_balance")
                result[name] = 0;
            else
                result[name] = "";
        }
        success("Vendor retrieved with ID:", id);
        return result;
    }

    // Retrieve record from database
    query.prepare("SELECT * FROM vendors WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        fail("Vendor retrieval failed: " + query.lastError().text());
        return result;
    } else if (!query.next()) {
        fail("No vendor found with ID: " + id);
        return result;
    }

    result["id"]             = query.value("id");
    result["name"]           = query.value("name");
    result["address1"]       = query.value("address1");
    result["address2"]       = query.value("address2");
    result["city"]           = query.value("city");
    result["state"]          = query.value("state");
    result["postal_code"]    = query.value("postal_code");
    result["phone"]          = query.value("phone");
    result["unpaid_balance"] = query.value("unpaid_balance");

    success("Vendor retrieved with ID:", id);
    return result;
}

/**
 * @brief Update vendor in database
 *
 * @param id Id of vendor to be updated
 * @param result Variant map containing the fields and their associated value to be updated
 * @return True if successful, otherwise false
 */
bool VendorAccess::update(const QString &id, const QVariantMap &vendorData) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        UPDATE vendors SET
            name = :name,
            address1 = :address1,
            address2 = :address2,
            city = :city,
            state = :state,
            postal_code = :postal_code,
            phone = :phone,
            unpaid_balance = :unpaid_balance
        WHERE id = :id
    )");

    query.bindValue(":id", id);
    query.bindValue(":name", vendorData["name"]);
    query.bindValue(":address1", vendorData["address1"]);
    query.bindValue(":address2", vendorData["address2"]);
    query.bindValue(":city", vendorData["city"]);
    query.bindValue(":state", vendorData["state"]);
    query.bindValue(":postal_code", vendorData["postal_code"]);
    query.bindValue(":phone", vendorData["phone"]);
    query.bindValue(":unpaid_balance", vendorData["unpaid_balance"]);

    if (!query.exec())
        return fail(query.lastError().text());

    return success("Vendor updated with ID:", id);
}

/**
 * @brief Delete vendor from database
 *
 * @param id Id of vendor to be deleted
 * @return True if successful, otherwise false
 */
bool VendorAccess::remove(const QString &id) {
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM vendors WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
        return fail(query.lastError().text());

    return success("Vendor deleted with ID:", id);
}

/**
 * @brief Generate requested number of sample vendor records
 *
 * Note: No signals are emitted from this method
 *
 * @param count Number of records to be generated
 * @return True if successful, otherwise false
 */
bool VendorAccess::generateSample(int count) {
    QStringList names = { "Acme Corp", "Globex", "Initech", "Umbrella", "Soylent", "Stark Industries", "Wayne Enterprises", "Wonka Inc", "Cyberdyne", "Tyrell Corp" };
    QStringList cities = { "Denver", "Seattle", "Austin", "Chicago", "Phoenix", "Boston", "Atlanta", "San Diego", "Portland", "Dallas" };
    QStringList states = { "CO", "WA", "TX", "IL", "AZ", "MA", "GA", "CA", "OR", "NY" };

    for (int i = 0; i < count; ++i) {
        QVariantMap vendor;
        QString guid = QUuid::createUuid().toString(QUuid::WithoutBraces);

        vendor["id"]             = guid;
        vendor["name"]           = names.at(QRandomGenerator::global()->bounded(names.size())) + QString(" %1").arg(i + 1);
        vendor["address1"]       = QString("%1 Main St").arg(QRandomGenerator::global()->bounded(100, 999));
        vendor["address2"]       = "";
        vendor["city"]           = cities.at(QRandomGenerator::global()->bounded(cities.size()));
        vendor["state"]          = states.at(QRandomGenerator::global()->bounded(states.size()));
        vendor["postal_code"]    = QString("%1").arg(QRandomGenerator::global()->bounded(10000, 99999));
        vendor["phone"]          = QString("555-%1").arg(QRandomGenerator::global()->bounded(1000, 9999));
        vendor["unpaid_balance"] = QRandomGenerator::global()->bounded(5000.0);

        if (!add(vendor))
            return false;
    }

    return true;
}
