import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import GlobalQmlSettings 1.0
import QtQuick.Layouts 1.12

import corpchat.models.contactsModel 1.0

ListView {
    id: list
    anchors.topMargin: 10
    property int btnSize: 45

    signal newConversation
    signal newDialog
    focus: true
    clip: true
    model: contactsModel
    highlightFollowsCurrentItem: false
    spacing: 10
    highlight: delegateHighlight
    signal selectedChanged(int idx)
    header: Component {

        //anchors.topMargin: 5
        //width: list.width
        ColumnLayout {

            id: columnLayout
            width: list.width
            spacing: 5
            Rectangle {
                id: delimiter
                width: list.width
                height: 20
                color: "transparent"
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumWidth: list.btnSize * 3
                Layout.preferredHeight: list.btnSize
                color: "transparent"
                Rectangle {
                    color: Material.background
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Новый дилог")
                        color: Material.accent
                        font.pixelSize: 15
                        font.bold: true
                    }
                    width: parent.width / 2 - 10
                    height: 50
                    anchors {
                        left: parent.left
                        leftMargin: 5
                    }
                    border.width: 2
                    border.color: Material.accent

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newDialog()
                        }
                    }
                }
                Rectangle {
                    color: Material.background
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Новый беседа")
                        color: Material.accent
                        font.pixelSize: 15
                        font.bold: true
                    }
                    width: parent.width / 2 - 8
                    height: 50
                    anchors {
                        right: parent.right
                        rightMargin: 5
                    }
                    border.width: 2
                    border.color: Material.accent
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newConversation()
                        }
                    }
                }
            }

            TextField {
                id: newContactInput

                property var accent: Material.color(Material.Teal)
                Material.accent: accent
                Layout.fillWidth: true
                width: parent.width - 20
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                }
                placeholderText: qsTr("Никнейм")

                Layout.minimumWidth: list.btnSize * 3
                Layout.preferredHeight: list.btnSize
            }
        }
    }

    Component {
        id: delegateHighlight
        Rectangle {
            id: highlightImpl
            width: parent.width
            height: 50 //contactsDelegate.height + 1
            color: "transparent"
            y: list.currentItem.y
            border.width: 1
            border.color: Material.color(Material.DeepOrange)
            z: 5
            Behavior on y {
                PropertyAnimation {
                    //easing.type: Easing.bezierCurve
                    duration: 250
                }
            }
        }
    }

    delegate: ContactsDelegate {
        id: contactsDelegate
        width: list.width
        //height: 40
        avatarSource: model.avatar
        username: model.nickname
        z: 3
        onClick: {
            list.currentIndex = index
            console.log("selected delegate #" + index)
            selectedChanged(index)
        }

        Connections {
            target: client
            function onNotifyMessage(sender) {
                console.log("Notifying on " + sender)
                if (sender === model.nickname)
                    notify()
            }
            Component.onCompleted: {
                client.qmlNotifyUnreadMessage(model.nickname)
            }
        }
    }
    Component.onCompleted: {
        selectedChanged.connect(contactsModel.indexChanged)
        //        list.currentIndex = 0
    }
}
