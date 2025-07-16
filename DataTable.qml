// DataTable.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: dataTable
    width: parent.width
    height: parent.height

    property alias tableModel: tableModel
    property int selectedIndex: -1
    // Track current sort column and direction
    property string sortField: "date"
    property bool sortAscending: true

    // 🔹 Action toolbar
    Rectangle {
        id: toolbar
        width: parent.width
        height: 50
        color: "#f0f0f0"
        border.color: "#cccccc"

        Row {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            padding: 10

            Button {
                text: "➕ Add Row"
                onClicked: {
                    tableModel.append({ date: "2025-07-08", amount: "$0", category: "New" })
                }
            }
            Button {
                text: "🗑️ Delete Row"
                enabled: selectedIndex >= 0
                onClicked: {
                    if (selectedIndex >= 0) tableModel.remove(selectedIndex)
                }
            }
        }
    }

    // 🔹 Header Row placed above ScrollView
    Rectangle {
        id: headerRow
        width: parent.width
        height: 40
        anchors.top: toolbar.bottom
        color: "#e0e0e0"
        border.color: "#aaaaaa"
        Row {
            spacing: 0  // let fixed widths control spacing
            anchors.fill: parent
            anchors.margins: 5

            // 📅 Date column with sort interaction
            Rectangle {
                width: 100
                height: parent.height
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: sortBy("date")
                }
                Text {
                    text: "Date" + (sortField === "date" ? (sortAscending ? " ▲" : " ▼") : "")
                    font.bold: true
                    anchors.centerIn: parent
                }
            }

            // 💰 Amount column
            Rectangle {
                width: 100
                height: parent.height
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: sortBy("amount")
                }
                Text {
                    text: "Amount" + (sortField === "amount" ? (sortAscending ? " ▲" : " ▼") : "")
                    font.bold: true
                    anchors.centerIn: parent
                }
            }

            // 🏷️ Category column
            Rectangle {
                width: 150
                height: parent.height
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: sortBy("category")
                }
                Text {
                    text: "Category" + (sortField === "category" ? (sortAscending ? " ▲" : " ▼") : "")
                    font.bold: true
                    anchors.centerIn: parent
                }
            }
        }
    }

    // 🔹 Scrollable Data Rows
    ScrollView {
        id: scrollArea
        anchors.top: headerRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Column {
            spacing: 2
            Repeater {
                model: tableModel
                delegate: Rectangle {
                    width: scrollArea.width
                    height: 40
                    color: index === dataTable.selectedIndex ? "#d0eaff" : "#ffffff"
                    border.color: "#cccccc"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: dataTable.selectedIndex = index
                    }

                    Row {
                        spacing: 10
                        anchors.fill: parent
                        anchors.margins: 5

                        Text { text: model.date; width: 100 }
                        Text { text: model.amount; width: 100 }
                        Text { text: model.category; width: 150 }
                    }
                }
            }
        }
    }

    ListModel {
        id: tableModel
    }

    function sortBy(field) {
        var i;

        if (sortField === field) {
            sortAscending = !sortAscending;
        } else {
            sortField = field;
            sortAscending = true;
        }

        // Collect model data into a JS array
        let rows = [];
        for (i = 0; i < tableModel.count; i++) {
            rows.push(tableModel.get(i));
        }

        // Sort the array
        rows.sort(function(a, b) {
            let va = a[field];
            let vb = b[field];

            // Handle date parsing if field is 'date'
            if (field === "date") {
                va = new Date(va);
                vb = new Date(vb);
            }

            return sortAscending ? (va > vb ? 1 : -1) : (va < vb ? 1 : -1);
        });

        // Reset the model
        tableModel.clear();
        for (i = 0; i < rows.length; i++) {
            tableModel.append(rows[i]);
        }
    }
}
