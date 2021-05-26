import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import GlobalQmlSettings 1.0
import MessengerForm.IconClickHighlight 1.0

Rectangle {
    id: root
    color: Qt.lighter(Material.backgroundColor)
    height: textArea.contentHeight + 25
    border.color: Material.backgroundColor
    radius: 8

    signal fileSelected
    signal messageSent

    FontLoader {
        id: emojiFont
        source: "qrc:/qml/fonts/OpenSansEmoji.ttf"
    }
    RowLayout {
        id: rowLayout
        spacing: 5
        anchors.fill: parent
        readonly property int btnSize: 28
        Rectangle {
            id: addFilesBtn
            z: 5
            Layout.leftMargin: 5
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.preferredWidth: rowLayout.btnSize
            Layout.preferredHeight: rowLayout.btnSize
            color: Qt.lighter(Material.backgroundColor)
            IconClickHighlight {
                id: fileHighlight
                maxRadius: rowLayout.btnSize + 5
            }
            Image {
                id: addFilesBtnImage
                width: rowLayout.btnSize
                height: rowLayout.btnSize
                anchors.centerIn: parent
                source: "qrc:/qml/icons/add_circle_outline-white-48dp"
                MouseArea {
                    id: fileBtnArea
                    anchors.fill: parent
                    onClicked: {
                        fileHighlight.run()
                        fileDialog.visible = true
                    }
                }
            }
            FileDialog {
                id: fileDialog
                nameFilters: ["Image files (*.png *.jpg)"]
                //selectMultiple: true
                title: qsTr("Выберите файл")
                folder: shortcuts.home
                onAccepted: {
                    console.log("You chose: " + fileDialog.fileUrls)

                    client.sendImage(fileUrls)
                }
                onRejected: {
                    fileDialog.visible = false
                }
            }
        } //addFilesBtn
        Flickable {
            id: scrollView

            Layout.preferredWidth: root.width - rowLayout.btnSize * 3 - rowLayout.spacing * 2 - 12
            Layout.fillHeight: true
            flickableDirection: Flickable.VerticalFlick
            interactive: false
            TextArea.flickable: TextArea {
                property var accent: Material.color(Material.DeepOrange)
                id: textArea

                font.family: emojiFont.name
                font.pointSize: 16
                selectionColor: Material.color(Material.LightBlue)
                selectedTextColor: "white"
                wrapMode: TextEdit.Wrap
                Material.accent: accent
                placeholderText: qsTr("Написать сообщение...")
                onTextChanged: {
                    if (textArea.text.length >= 500) {
                        textArea.text = textArea.text.substring(0, 500)
                    }
                }
            }
        }
        Rectangle {
            id: emojiBtn
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            Layout.preferredWidth: rowLayout.btnSize
            Layout.preferredHeight: rowLayout.btnSize
            color: Qt.lighter(Material.backgroundColor)
            IconClickHighlight {
                id: emojiHighlight
                maxRadius: rowLayout.btnSize + 5
            }

            Image {
                id: emojiBtnImage
                width: rowLayout.btnSize
                height: rowLayout.btnSize
                anchors.centerIn: parent
                source: "qrc:/qml/icons/insert_emoticon-white-48dp"

                MouseArea {
                    id: emojiBtnArea
                    anchors.fill: parent
                    onClicked: {
                        emojiHighlight.run()
                    }
                }
            }
        }
        Rectangle {
            id: sendBtn

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.preferredWidth: rowLayout.btnSize
            Layout.preferredHeight: rowLayout.btnSize
            color: Qt.lighter(Material.backgroundColor)
            IconClickHighlight {
                id: sendHighlight
                maxRadius: rowLayout.btnSize + 5
            }

            Image {
                anchors.centerIn: parent
                source: "qrc:/qml/icons/send-white-48dp"
                width: rowLayout.btnSize //parent.width / 2 + parent.width / 4
                height: rowLayout.btnSize //parent.height / 2 + parent.height / 4
            }
            MouseArea {
                id: sendBtnArea
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    //highlightAnim.running = true
                    if (textArea.text != "") {
                        sendHighlight.run()
                        client.sendMessage(textArea.text)
                        messageSent()
                        textArea.text = ""
                    }
                }
            }
        }
    }
}
