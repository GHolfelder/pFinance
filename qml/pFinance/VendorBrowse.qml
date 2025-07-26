import QtQuick 2.15
import QtQuick.Controls 2.15
import "components"

Item {
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    Column {
        anchors.fill: parent
        spacing: 10
        // padding: 20

        Label {
            text: "🧮 Vendor List"
            font.bold: true
            font.pointSize: 18
        }

        DataTable {
            model: vendorModel
            sortOrder: vendorModel.sortOrder
            sortColumn: vendorModel.sortColumn
            onSortRequested: (columnName) => {
                vendorModel.sortBy(columnName);
            }
        }
    }
}
