// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import "components"

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "Financial Tracker"

    DataTable {
        anchors.fill: parent
        model: vendorModel
    }
}
