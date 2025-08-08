import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "."

Item {
    id: dataTable
    width: parent.width
    height: parent.height

    // Custom properties
    property var model                      // Passed in model used for browse
    property var access                     // Passed in access used to delete row
    property string form                    // Name of form to use for maintenance
    property int sortOrder                  // Current sort order (ascending / descending)
    property string sortColumn              // Current sort column
    property int selectedRow: -1            // Selected row in grid
    property string selectedId: ""          // Id of selected row in grid
    property int minimumColumnWidth: 80     // Minimum column wodth
    signal sortRequested(var columnName)    // Signal emitted when a column header is clicked

    // When a column is clicked
    onSortRequested: (columnName) => {
        selectedRow = model.sortBy(columnName, selectedId);
        sortOrder   = model.sortOrder
        sortColumn  = model.sortColumn
    }

    // When component is loaded
    Component.onCompleted: {
        selectedRow = model.sortBy(model.defaultSort(), selectedId);
        sortOrder   = model.sortOrder
        sortColumn  = model.sortColumn
    }

    /*
     * Refresh grid after form is closed
     *
     * @param lastId Id of last record added or changed
     */
    function refresh(lastId) {
        if (lastId !== "") {
            selectedRow = model.refresh(lastId)
            if (selectedRow >= 0) selectedId = lastId;
        }
        stackView.pop()
    }

    // Delete confirmation box
    MsgBox {
        id: msgBox
        title: "Confirm"
        message: "Record about to be deleted. Are you sure you wish to continue?"
        buttons: Dialog.Yes | Dialog.No
        onAccepted: {
            access.remove(selectedId)
            refresh(selectedId)
        }
    }

    // Popup showing column visibility settings
    Popup {
        id: columnPopup
        modal: true
        x: settingsButton.x
        y: settingsButton.y + settingsButton.height
        width: 320
        height: implicitHeight

        focus: true

        // ✅ Proper background assignment
        background: Rectangle {
            color: "#f0f0f0"
            radius: 6
            border.color: "#cccccc"
            border.width: 1
        }

        // ✅ Proper content assignment
        contentItem: ColumnSelector {
            columnNames: model.columnNames
            columnTitles: model.columnTitles
            visibleColumns: model.visibleColumns

            onVisibilityUpdated: (updatedList) => {
                model.setVisibleColumns(updatedList)
            }
        }
    }

    // Action toolbar
    Rectangle {
        id: toolbar
        width: parent.width
        height: 55
        z: 20

        Row {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            padding: 10

            Button {
                id: settingsButton
                text: "⚙️"
                onClicked: columnPopup.open()
            }
            Button {
                text: "➕ Add"
                width: 150
                onClicked: {
                    const formItem = stackView.push(Qt.resolvedUrl(form), { editId: "" })
                    formItem.formClosed.connect((lastId) => refresh(lastId))
                }
            }
            Button {
                text: "✏️ Change"
                width: 150
                enabled: selectedRow >= 0
                onClicked: {
                    const formItem = stackView.push(Qt.resolvedUrl(form), { editId: selectedId })
                    formItem.formClosed.connect((lastId) => refresh(lastId))
                }
            }
            Button {
                text: "🗑️ Delete"
                width: 150
                enabled: selectedRow >= 0
                onClicked: {
                    msgBox.open()
                }
            }
        }
    }

    // Grid column titles
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

            Row {
                anchors.centerIn: parent
                spacing: 4

                Text {
                    visible: sortColumn === textItem.columnName
                    text: sortOrder === 0 ? "▲" : "▼"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    color: "#555"
                    anchors.verticalCenter: textItem.verticalCenter
                }
                Text {
                    id: textItem
                    text: model.display
                    font.bold: true
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    horizontalAlignment: Text.AlignHCenter
                    property int columnIndex: model.column   // Save column index
                    property var columnName: model.cellname  // Save column name
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: sortRequested(textItem.columnName);
            }
        }
    }

    // Griod row titles
    VerticalHeaderView {
        id: verticalHeader
        anchors.top: horizontalHeader.bottom
        anchors.left: parent.left
        syncView: table
        clip: true
        z: 10
    }

    // TableView
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
                    selectedId = model.id;
                }
            }
        }
    }
}
