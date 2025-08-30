// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    ColumnLayout {
        anchors.fill: parent

        // Persistent header with hamburger
        ToolBar {
            Layout.fillWidth: true
            RowLayout {
                spacing: 10

                ToolButton {
                    text: "\u2630" // Unicode hamburger
                    onClicked: drawer.open()
                }

                Label {
                    text: stackView.currentItem?.title ?? "Welcome"
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        // StackView for page navigation
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: Component {
                Loader {
                    source: Qt.resolvedUrl("WelcomePage.qml")
                }
            }
        }
    }

    // Optional drawer for navigation
    Drawer {
        id: drawer
        width: 200
        height: parent.height

        Column {
            ItemDelegate {
                text: "🏠 Home"
                onClicked: {
                    stackView.clear()
                    stackView.push(Qt.resolvedUrl("WelcomePage.qml"))
                    drawer.close()
                }
            }
            ItemDelegate {
                text: "🧮 Categories"
                onClicked: {
                    stackView.clear()
                    stackView.push(Qt.resolvedUrl("CategoryBrowse.qml"))
                    drawer.close()
                }
            }
            ItemDelegate {
                text: "🧮 Vendors"
                onClicked: {
                    stackView.clear()
                    stackView.push(Qt.resolvedUrl("VendorBrowse.qml"))
                    drawer.close()
                }
            }
        }
    }
}
