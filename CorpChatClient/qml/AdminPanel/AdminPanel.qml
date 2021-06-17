import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Dialogs 1.2
import GlobalQmlSettings 1.0

import corpchat.net.client 1.0

import corpchat.models.usersModel 1.0

Rectangle {
    id: root
    color: Material.background
    signal back

    property string nickname
    property string email
    property bool isBan
    property string imgUrl
    Dialog {
        id: submitDataUpdate
        visible: false

        Rectangle {
            color: Material.backgroundColor
            implicitWidth: 400
            implicitHeight: 100
            Label {
                anchors.centerIn: parent
                text: "Вы уверены что хотите изменить данные пользователя?"
            }
        }

        title: "Подтверждение"

        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onButtonClicked: {
            if (clickedButton == StandardButton.Ok) {
                console.log("Accepted " + clickedButton)
                client.updateUserData(root.nickname, root.email, root.imgUrl,
                                      root.isBan)
            } else {
                console.log("Rejected" + clickedButton)
                submitDataUpdate.visible = false
            }
        }

        Connections {
            Component.onCompleted: console.log("dialog Created!")
            target: submit_btn
            onClicked: {
                console.log("Dialog is opened")
                if (root.imgUrl != new_avatar.source
                        || root.nickname != nickname_fld.text
                        || root.isBan !== ban_check.checkState === Qt.Checked) {
                    submitDataUpdate.visible = true
                    console.log(root.imgUrl + " " + root.nickname + " " + root.isBan)
                    root.imgUrl = new_avatar.source
                    root.nickname = nickname_fld.text
                    root.email = email_lbl.text
                    root.isBan = ban_check.checkState == Qt.Checked
                }
            }
        }
    }

    Rectangle {
        width: parent.width / 2
        height: parent.height
        color: "red"
        anchors {
            left: parent.left
        }

        Rectangle {
            id: newConversationInitialItem
            z: 4
            anchors.fill: parent
            color: Material.background
            TextField {
                id: searchField
                anchors {
                    //left: parent.left
                    top: parent.top
                    //leftMargin: 10
                    topMargin: 10
                    left: parent.left
                    leftMargin: 20
                }
                width: 300
                placeholderText: "nickname"
            }
            ToolButton {
                width: 45
                height: 45
                anchors.left: searchField.right
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 10
                contentItem: Image {
                    source: "qrc:/qml/icons/iconfinder_314807_search_icon"
                }
                onClicked: {
                    if (searchField.text !== "") {
                        contactChooseModel.setFilterFixedString(
                                    searchField.text)
                    } else
                        searchField.placeholderTextColor = Material.color(
                                    Material.Red)
                }
            }

            ListView {
                id: otherContactListView
                width: searchField.width + 45
                anchors.top: searchField.bottom
                anchors.bottom: back_btn.top
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                spacing: 3
                model: adminModel
                delegate: Rectangle {
                    id: delegate
                    property string nickname: model.nickname
                    property string email: model.email
                    property bool isBan: model.ban
                    property string imgUrl: model.avatar

                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    implicitHeight: 80

                    color: Material.background
                    Image {
                        id: avatar
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 30
                        }
                        source: "file:///" + imgUrl
                        height: 50
                        width: 50
                    }
                    Label {
                        anchors.bottom: parent.verticalCenter
                        anchors.bottomMargin: 5
                        anchors.left: avatar.right
                        anchors.leftMargin: 10
                        text: nickname
                    }
                    Label {
                        anchors.top: parent.verticalCenter
                        anchors.topMargin: 5
                        anchors.left: avatar.right
                        anchors.leftMargin: 10
                        text: email
                        opacity: 0.4
                    }

                    Text {
                        id: isBaned
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        visible: true
                        anchors.rightMargin: 20
                        font.bold: true
                        font.pixelSize: 20
                        text: model.ban ? "Baned" : "Unbaned"
                        color: model.ban ? "red" : "green"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("789")
                            new_data.visible = true
                            new_avatar.source = "file:///" + delegate.imgUrl
                            nickname_fld.text = delegate.nickname
                            email_lbl.text = delegate.email
                            ban_check.checkState = delegate.isBan ? Qt.Checked : Qt.Unchecked

                            root.imgUrl = delegate.imgUrl
                            root.nickname = delegate.nickname
                            root.email = delegate.email
                            root.isBan = delegate.isBan
                        }
                    }
                }
            }

            Button {
                id: back_btn
                anchors {
                    horizontalCenter: otherContactListView.horizontalCenter
                    leftMargin: 20
                    bottom: parent.bottom
                    bottomMargin: 20
                }

                text: qsTr("back")
                implicitWidth: 150
                onClicked: {
                    back()
                }
            }
        }
    }
    Rectangle {
        id: new_data
        visible: false
        height: parent.height
        width: parent.width - newConversationInitialItem.width
        color: Material.background
        anchors.right: parent.right
        anchors.top: parent.top
        Image {
            id: new_avatar
            width: parent.width / 2 - 45
            height: width
            source: "file:///" + applicationDirPath + "/../images/Default.jpg"
            anchors {
                //                horizontalCenter: parent.horizontalCenter
                right: parent.right
                rightMargin: 20
                top: parent.top
                topMargin: 20
            }
        }
        Button {
            id: drop_img_btn
            anchors {
                right: new_avatar.left
                rightMargin: 20
                top: parent.top
                topMargin: 20
            }
            text: qsTr("drop images")

            implicitWidth: 150
            implicitHeight: 50
            onClicked: {
                new_avatar.source = "file:///" + applicationDirPath + "/../images/Default.jpg"
            }
        }

        CheckBox {
            id: ban_check
            anchors {
                right: new_avatar.left
                rightMargin: 20
                top: drop_img_btn.bottom
                topMargin: 20
            }
            text: qsTr("Ban")

            implicitWidth: 150
            implicitHeight: 50
        }

        TextField {
            id: nickname_fld
            placeholderText: qsTr("nickname")
            width: new_avatar.width
            implicitHeight: 50
            anchors {
                top: new_avatar.bottom
                topMargin: 20
                right: parent.right
                rightMargin: 20
            }
        }
        Label {
            id: email_lbl
            text: qsTr("email")
            anchors {
                top: nickname_fld.bottom
                topMargin: 20
                right: parent.right
                rightMargin: 20
            }
        }

        Button {
            id: submit_btn
            anchors {
                right: parent.right
                rightMargin: 20
                bottom: parent.bottom
                bottomMargin: 20
            }

            text: qsTr("summit")

            implicitWidth: 150
            implicitHeight: 50
            onClicked: {

            }
        }
    }
}
