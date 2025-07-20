import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dataTable
    width: parent.width
    height: parent.height

    property var model

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: table.left
        anchors.top: parent.top
        syncView: table
        clip: true
        z: 10
    }

    VerticalHeaderView {
        id: verticalHeader
        anchors.top: table.top
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

        delegate:  Label {
            text: model.tabledata
            width: 100
            padding: 12

            Rectangle {
                anchors.fill: parent
                color: "#efefef"
                z: -1
            }
        }
    }
}
