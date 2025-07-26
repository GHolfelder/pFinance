import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 640
    height: 480

    Column {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "👋 Welcome to Personal Finance"
            font.pointSize: 20
            font.bold: true
        }

        Text {
            text: "Use the menu access your data."
            wrapMode: Text.WordWrap
            width: parent.width * 0.8
        }
    }
}
