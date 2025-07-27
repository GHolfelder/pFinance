import QtQuick
import QtQuick.Controls
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

        // Toast message area
        Toast {
            id: toast
        }

        // After form is open
        Component.onCompleted: {
            if (editId !== "") {
                vendorData = vendorAccess.get(editId)
            }
            isLoading = false;
        }

        // Connect to signals from access class
        Connections {
            target: vendorAccess

            function onOperationSuccess (id) {
                if (!isLoading) {
                    lastId = id
                    if (editId === "") {
                        toast.show("✅ Vendor added successfully")
                        vendorData = {}       // Reset form data
                        editId = ""           // Ensure we're in 'add' mode
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
        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20             // Adds left/right spacing
            spacing: 12

            FormField {
                labelText: "Vendor Name"
                fieldText: vendorData.name || ""
                onTextChanged: vendorData.name = fieldText
            }
            FormField {
                labelText: "Address 1"
                fieldText: vendorData.address1 || ""
                onTextChanged: vendorData.address1 = fieldText
            }
            FormField {
                labelText: "Address 2"
                fieldText: vendorData.address2 || ""
                onTextChanged: vendorData.address2 = fieldText
            }
            FormField {
                labelText: "City"
                fieldText: vendorData.city || ""
                onTextChanged: vendorData.city = fieldText
            }
            FormField {
                labelText: "State"
                fieldText: vendorData.state || ""
                onTextChanged: vendorData.state = fieldText
            }
            FormField {
                labelText: "Postal Code"
                fieldText: vendorData.postal_code || ""
                onTextChanged: vendorData.postal_code = fieldText
            }
        }
    }
}
