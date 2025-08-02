import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"

Item {
    id: vendorForm

    property var vendorData: ({})           // Holds QVariantMap with editable values
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
        vendorData = vendorAccess.get(editId)
        firstField.focus = true
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
            text: editId === "" ? "➕ Add Vendor" : "✏️ Changing Vendor"
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
                            vendorAccess.add(vendorData)
                        else
                            vendorAccess.update(editId, vendorData)
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
            target: vendorAccess

            function onOperationSuccess (id) {
                if (isLoading) {
                    isLoading = false
                    firstField.focus = true
                } else {
                    lastId = id
                    if (editId === "") {
                        toast.show("✅ Vendor added successfully")
                        isLoading = true            // Start loading process
                        editId = ""                 // Ensure we're in 'add' mode
                        vendorData = vendorAccess.get(editId)
                    } else {
                        formClosed(lastId)
                    }
                    formSaved(vendorData)
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
            property int textWidth: 300     // Shared width for all labels

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20             // Adds left/right spacing
            spacing: 12

            RowLayout {
                Label {
                    text: "Vendor name"
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                TextField {
                    id: firstField
                    text: vendorData.name
                    Layout.preferredWidth: layoutParent.textWidth
                    onTextChanged: vendorData.name = text
                }
            }
            RowLayout {
                Label {
                    text: "Address 1"
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                TextField {
                    text: vendorData.address1
                    Layout.preferredWidth: layoutParent.textWidth
                    onTextChanged: vendorData.address1 = text
                }
            }
            RowLayout {
                Label {
                    text: "Address 2"
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                TextField {
                    text: vendorData.address2
                    Layout.preferredWidth: layoutParent.textWidth
                    onTextChanged: vendorData.address2 = text
                }
            }
            RowLayout {
                Label {
                    text: "City, state and postal code"
                    Layout.preferredWidth: layoutParent.labelWidth
                }
                TextField {
                    text: vendorData.city
                    Layout.preferredWidth: (layoutParent.textWidth - 20) / 3
                    onTextChanged: vendorData.city = text
                }
                TextField {
                    text: vendorData.state
                    Layout.preferredWidth: (layoutParent.textWidth - 20) / 3
                    Layout.leftMargin: 5
                    onTextChanged: vendorData.state = text
                }
                TextField {
                    text: vendorData.postal_code
                    Layout.preferredWidth: (layoutParent.textWidth - 20) / 3
                    Layout.leftMargin: 5
                    onTextChanged: vendorData.postal_code = text
                }
            }
        }
    }
}
