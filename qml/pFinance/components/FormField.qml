import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Column {
    id: root
    property string labelText: ""           // Label for field
    property string placeholder: ""         // Placeholder text for entry field
    property string fieldType: "text"       // "text" or "date"
    property bool isPassword: false         // True if password field (text only)
    property bool readOnly: false           // True if read-only
    property int fieldWidth: 200            // width of entry field
    property string fieldText: ""
    property date selectedDate: new Date()

    signal textChanged(string newText)
    signal dateChanged(date newDate)

    spacing: 4

    Label {
        text: labelText
        font.pixelSize: 14
        color: "#666"
    }

    Loader {
        id: fieldLoader
        active: true
        sourceComponent: fieldType === "date" ? dateField : textField
    }

    Component {
        id: textField
        TextField {
            width: fieldWidth
            text: root.fieldText
            placeholderText: root.placeholder
            echoMode: root.isPassword ? TextInput.Password : TextInput.Normal
            readOnly: root.readOnly
            onTextChanged: {
                root.fieldText = text
                root.textChanged(text)
            }
        }
    }

    Component {
        id: dateField
        TextField {
            width: fieldWidth
            text: root.fieldText
            placeholderText: root.placeholder
            readOnly: root.readOnly
            onTextChanged: {
                root.fieldText = text
                root.textChanged(text)
            }
        }
    }
}
