import QtQuick 2.15
import QtQuick.Controls 2.15
import "components"

Item {
    id: vendorBrowse
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    Column {
        anchors.fill: parent
        spacing: 10

        Label {
            text: "🧮 Vendor List"
            font.bold: true
            font.pointSize: 18
        }

        DataTable {
            model: vendorModel
            form: "../VendorForm.qml"
        }
    }
}
