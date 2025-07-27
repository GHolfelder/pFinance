import QtQuick
import QtQuick.Controls

Dialog {
    id: msgBox
    title: "Message"
    modal: true
    focus: true
    property string message: ""
    property alias textColor: msgText.color
    property alias backgroundColor: background.color

    width: 400
    x: (parent?.width - width) / 2
    y: (parent?.height - implicitHeight) / 2
    visible: false

    contentItem: Rectangle {
        id: background
        color: "#f8f8f8"
        implicitHeight: msgText.implicitHeight + 48
        implicitWidth: Math.min(msgText.implicitWidth + 48, 400)

        Text {
            id: msgText
            text: msgBox.message
            wrapMode: Text.Wrap
            anchors.centerIn: parent
            anchors.margins: 24
            color: "#333"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width - 48
        }
    }

    standardButtons: Dialog.Ok
    onAccepted: msgBox.visible = false
}
