import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dataTable
    width: parent.width
    height: parent.height

    property var model
    property int selectedIndex: -1

    signal rowSelected(int index)
    onRowSelected: function(index) {
        selectedIndex = index;
        console.log(selectedIndex);
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 🔹 Toolbar
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#f0f0f0"
            border.color: "#cccccc"

            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                spacing: 10

                Button { text: "➕ Add" }
                Button { text: "✏️ Update" }
                Button {
                    text: "🗑️ Delete"
                    enabled: selectedIndex >= 0
                }
            }
        }

        // 🔹 TableView
        TableView {
            id: table
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: dataTable.model
            clip: true
            columnSpacing: 2
            rowSpacing: 2

            selectionModel: ItemSelectionModel {
                model: dataTable.model
            }

            delegate: Rectangle {
                id: rowRect
                implicitHeight: 40
                implicitWidth: 350
                border.color: "#dddddd"
                color: selected ? "#d0eaff" : "#ffffff"

                required property bool selected

                MouseArea {
                    anchors.fill: parent
                    onClicked: tableView.selectionModel.select(tableView.model.index(index, 0), ItemSelectionModel.Select)
                }

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 10

                    Text { text: name;      width: 100 }
                    Text { text: address1;  width: 100 }
                    Text { text: city;      width: 150 }
                }
            }
        }
    }
}
