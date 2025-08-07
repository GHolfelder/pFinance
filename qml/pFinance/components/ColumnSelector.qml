// ColumnSelector.qml
import QtQuick
import QtQuick.Controls

Control {
    id: columnSelector
    width: 300
    height: implicitHeight
    padding: 12

    property var columnNames: []         // e.g. ["id", "name", "email"]
    property var columnTitles: []        // e.g. ["ID", "Name", "Email"]
    property var visibleColumns: []      // e.g. ["name", "email"]
    signal visibilityUpdated(var updatedList)

    background: Rectangle {
        color: "#ffffff"
        radius: 6
        border.color: "#cccccc"
        border.width: 1
    }

    contentItem: Column {
        spacing: 8

        TextField {
            id: filterField
            placeholderText: "Filter columns..."
        }

        Label {
            text: "Show / Hide Columns"
            font.bold: true
        }

        Repeater {
            id: columnRepeater
            model: columnNames.length

            delegate: CheckBox {
                id: checkBox
                text: columnTitles[index].replace("\n", " ")
                checked: visibleColumns.indexOf(columnNames[index]) >= 0
                visible: columnTitles[index].toLowerCase().includes(filterField.text.toLowerCase())

                onClicked: {
                    let updated = [...visibleColumns];
                    const name = columnNames[index];
                    if (checked && !updated.includes(name)) {
                        updated.push(name);
                    } else if (!checked && updated.includes(name)) {
                        updated.splice(updated.indexOf(name), 1);
                    }
                    visibilityUpdated(updated);
                }
            }
        }
    }
}
