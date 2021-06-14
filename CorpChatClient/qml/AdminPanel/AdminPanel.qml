import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0
import QtQuick.Dialogs 1.2
import GlobalQmlSettings 1.0

import corpchat.models.contactsModel 1.0
import corpchat.models.messagesModel 1.0

import corpchat.models.contactschooseModel 1.0

Rectangle {
    color: Material.background
    Rectangle {
        width: parent.width / 2
        height: parent.height
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
                    horizontalCenter: parent.horizontalCenter
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
                width: parent.width
                anchors.top: searchField.bottom
                anchors.bottom: otherContactButtonGroup.top
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                spacing: 3
                model: contactChooseModel
                delegate: Rectangle {
                    width: otherContactListView.width
                    implicitHeight: 80

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("789")
                            console.log(" choose mode = " + model.isChoose)
                            if (model.isChoose) {
                                isChecked.visible = false
                                model.isChoose = false
                            } else {
                                isChecked.visible = true
                                model.isChoose = true
                            }
                        }
                    }

                    //                                border.width: 1
                    //                                border.color: Material.accent
                    color: Material.background
                    Image {
                        id: avatar
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 30
                        }
                        source: "file:///" + model.avatar
                        height: 50
                        width: 50
                    }
                    Label {
                        anchors.bottom: parent.verticalCenter
                        anchors.bottomMargin: 5
                        anchors.left: avatar.right
                        anchors.leftMargin: 10
                        text: model.nickname
                    }
                    Label {
                        anchors.top: parent.verticalCenter
                        anchors.topMargin: 5
                        anchors.left: avatar.right
                        anchors.leftMargin: 10
                        text: model.email
                        opacity: 0.4
                    }
                    Rectangle {
                        id: isChecked
                        visible: false
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: avatar.right
                            leftMargin: 330
                            //rightMargin: 30
                        }
                        color: "lime"
                        radius: width / 2
                        height: 40
                        width: 40
                        Image {
                            source: "qrc:/qml/icons/check-mark"
                            anchors.centerIn: parent
                            width: parent.width - 10
                            height: parent.height - 10
                        }
                    }
                }
            }

            Rectangle {
                id: otherContactButtonGroup
                color: "transparent"
                width: parent.width
                implicitHeight: 50
                anchors.bottom: parent.bottom
                Button {
                    anchors {
                        left: parent.left
                        leftMargin: 75
                    }

                    text: qsTr("cancel")
                    implicitWidth: 150
                    onClicked: {
                        animationOpacityBGConversationClose.start()
                        animationScaleConversationClose.start()
                    }
                }
                Button {
                    anchors {
                        right: parent.right
                        rightMargin: 75
                    }

                    text: qsTr("submit")
                    implicitWidth: 150
                    onClicked: {

                        newConversationStackView.push(inputTitleandAvatar)
                    }
                }
            }
        }
    }
}
