import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: toast
    property alias message: textItem.text
    property int duration: 2000
    width: parent ? parent.width : 300
    height: 60
    visible: false

    signal dismissed()

    // Fade animation
    Behavior on opacity {
        NumberAnimation {
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    Rectangle {
        id: background
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        width: textItem.implicitWidth + 40
        height: 40
        radius: 6
        color: "#444"
        opacity: 0.9

        Text {
            id: textItem
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 16
        }
    }

    Timer {
        id: hideTimer
        interval: duration
        running: false
        onTriggered: {
            toast.opacity = 0
            toast.visible = false
            dismissed()
        }
    }

    function show(msg) {
        message = msg
        visible = true
        opacity = 1
        hideTimer.restart()
    }
}
