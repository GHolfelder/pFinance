import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"

Item {
    id: categoryForm

    property var categoryData: ({})         // Holds QVariantMap with editable values
    property string editId: ""              // Id of record to be edited
    property string lastId: ""              // Id of last record added or edited
    property bool isLoading: true           // True when form is being loaded, otherwise false

    signal formSaved(var data)
    signal formClosed(string lastId)

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    // After form is open
    Component.onCompleted: {
        isLoading = true
        categoryData = categoryAccess.get(editId)
    }

    Toast {
        id: toast
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom      // Or wherever you want it to appear
        z: 999                             // Ensure it floats above other content
        visible: false                     // Default hidden
    }

    Column {
        anchors.fill: parent
        spacing: 10

        Label {
            text: editId === "" ? "➕ Add Category" : "✏️ Changing Category"
            font.bold: true
            font.pointSize: 18
        }

        // Action toolbar
        Rectangle {
            id: toolbar
            width: parent.width
            height: 55

            Row {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                padding: 10

                Button {
                    text: "✅ Save"
                    width: 150
                    onClicked: {
                        if (editId == "")
                            categoryAccess.add(categoryData)
                        else
                            categoryAccess.update(editId, categoryData)
                    }
                }
                Button {
                    text: "❌ Cancel"
                    width: 150
                    onClicked: {
                        formClosed(lastId)
                    }
                }
            }
        }

        // Message box
        MsgBox {
            id: msgBox
            title: "Operation Failed"
        }

        // Connect to signals from access class
        Connections {
            target: categoryAccess

            function onOperationSuccess (id) {
                if (isLoading) {
                    isLoading = false
                } else {
                    lastId = id
                    if (editId === "") {
                        toast.show("✅ Category added successfully")
                        isLoading = true            // Start loading process
                        editId = ""                 // Ensure we're in 'add' mode
                        categoryData = categoryAccess.get(editId)
                    } else {
                        formClosed(lastId)
                    }
                    formSaved(categoryData)
                }
            }

            // Show error message or banner
            function onOperationFailed (error) {
                msgBox.message = error
                msgBox.open()
            }
        }

        // Input Fields with left padding
        ColumnLayout {
            id: layoutParent
            property int labelWidth: 150    // Shared width for all labels
            property int textWidth: 300     // Shared width for all entry fields

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20             // Adds left/right spacing
            spacing: 12

            RowLayout {
                Label {
                    text: qsTr("Category name")
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                TextField {
                    text: categoryData.name
                    Layout.preferredWidth: layoutParent.textWidth
                    onTextChanged: categoryData.name = text
                    focus: true
                }
            }
            RowLayout {
                Label {
                    text: qsTr("Description")
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                ScrollView {
                    Layout.preferredWidth: layoutParent.textWidth
                    Layout.preferredHeight: 150

                    TextArea {
                        text: categoryData.description
                        wrapMode: TextArea.Wrap
                        onTextChanged: categoryData.description = text
                    }
                }
            }
            RowLayout {
                Label {
                    text: qsTr("Type")
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                SchemaComboBox {
                    columnName: "type"
                    accessObject: categoryAccess
                    selectedValue: categoryData.type
                    preferredWidth: layoutParent.textWidth

                    onSelectedValueChanged: categoryData.type = selectedValue
                }
            }
        }
    }
}
