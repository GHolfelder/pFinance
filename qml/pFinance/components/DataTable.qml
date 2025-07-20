import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dataTable
    width: parent.width
    height: parent.height

    // Custom properties
    property var model                      // Passed in model
    property int selectedRow: -1            // Selected row in grid
    property int minimumColumnWidth: 90     // Minimum column wodth

    // Action toolbar
    Rectangle {
        id: toolbar
        width: parent.width
        height: 55
        color: "#f0f0f0"
        border.color: "#cccccc"
        z: 20

        Row {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            padding: 10

            Button {
                text: "➕ Add"
                width: 150
                onClicked: {
                    console.log("Add row")
                }
            }
            Button {
                text: "✏️ Change"
                width: 150
                enabled: selectedRow > 0
                onClicked: {
                    console.log("Change row")
                }
            }
            Button {
                text: "🗑️ Delete"
                width: 150
                enabled: selectedRow > 0
                onClicked: {
                    console.log("Delete row")
                }
            }
        }
    }

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: table.left
        anchors.top: toolbar.bottom
        syncView: table
        clip: true
        z: 10
        delegate: Rectangle {
            implicitWidth: Math.max(minimumColumnWidth, textItem.implicitWidth)
            implicitHeight: 40
            color: "#dddddd"

            Text {
                id: textItem
                text: model.display
                anchors.centerIn: parent
                elide: Text.ElideRight
                clip: true
            }
        }
    }

    VerticalHeaderView {
        id: verticalHeader
        anchors.top: horizontalHeader.bottom
        anchors.left: parent.left
        syncView: table
        clip: true
        z: 10
    }

    // 🔹 TableView
    TableView {
        id: table
        model: dataTable.model

        anchors.left: verticalHeader.right
        anchors.top: horizontalHeader.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        columnSpacing: 1
        rowSpacing: 1
        z: 5
        boundsBehavior: Flickable.StopAtBounds

        // Set to 80px minimum width
        columnWidthProvider: function(column) {
            let contentWidth = table.implicitColumnWidth(column);
            return Math.max(minimumColumnWidth, contentWidth);
        }

        // Trigger layout recalculation when needed
        Component.onCompleted: table.forceLayout()

        delegate:  Label {
            text: model.celldata
            width: 100
            padding: 12

            Rectangle {
                anchors.fill: parent
                color: selectedRow === model.row
                    ? "#cce5ff"    // selected row color (light blue)
                    : model.row % 2 === 0
                        ? "#ffffff"  // even rows
                        : "#f9f9f9"  // odd rows
                z: -1
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedRow = model.row;
                }
            }
        }
    }
}
