// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import "."

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "Financial Tracker"

    DataTable {
        anchors.fill: parent

        Component.onCompleted: {
            tableModel.append({ date: "2025-07-07", amount: "$110", category: "Utilities" })
            tableModel.append({ date: "2025-07-06", amount: "$42", category: "Groceries" })
            tableModel.append({ date: "2025-07-05", amount: "$51.25", category: "Hardware" })
            tableModel.append({ date: "2025-07-04", amount: "$90.11", category: "Gasoline" })
            tableModel.append({ date: "2025-07-04", amount: "$7.80", category: "Groceries" })
            tableModel.append({ date: "2025-07-03", amount: "$99.99", category: "Animal feed" })
            tableModel.append({ date: "2025-07-03", amount: "$1001.11", category: "Hardware" })
            tableModel.append({ date: "2025-07-02", amount: "$123.00", category: "Dining" })
        }
    }
}
