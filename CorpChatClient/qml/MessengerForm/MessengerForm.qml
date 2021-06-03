import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0
import GlobalQmlSettings 1.0
import MessengerForm.MessageInputField 1.0
import MessengerForm.MessagesView 1.0
import MessengerForm.ContactsView 1.0

import corpchat.models.contactsModel 1.0
import corpchat.models.messagesModel 1.0

Rectangle {
    id: root
    height: GlobalSettings.defaultFormHeight
    width: GlobalSettings.defaultFormWidth
    FontLoader {
        id: starsetFont
        source: "../fonts/FARRAY.otf"
    }
    FontLoader {
        id: emojiFont
        source: "../fonts/OpenSansEmoji.ttf"
    }
    Rectangle {
        id: imageClickForm
        visible: false
        opacity: 1
        color: Qt.rgba(0, 0, 0, 0.75)
        anchors.fill: parent
        anchors.centerIn: parent
        z: 2
        layer.enabled: true
        MouseArea {
            id: close
            anchors.fill: parent
            onClicked: {
                animationScaleImageClose.start()
                animationOpacityBGClose.start()
            }
        }
        Image {
            id: dialog
            z: 3
            opacity: 1
            width: sourceSize.width < 900 ? sourceSize.width : 900
            height: sourceSize.height < 560 ? sourceSize.height : 560
            anchors.centerIn: parent
        }

        ColorAnimation {
            id: animationOpacityBGOpen
            target: imageClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.3)
            to: Qt.rgba(0, 0, 0, 0.75)
            duration: 100
        }
        ColorAnimation {
            id: animationOpacityBGClose
            target: imageClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.75)
            to: Qt.rgba(0, 0, 0, 0.3)
            duration: 99
        }
        ScaleAnimator {
            id: animationScaleImageOpen
            target: dialog
            from: 0.4
            to: 1
            duration: 100
        }
        ScaleAnimator {
            id: animationScaleImageClose
            target: dialog
            from: 1
            to: 0.4
            duration: 100
            onFinished: {
                imageClickForm.visible = false
            }
        }
    }

    SplitView {
        id: splitView
        anchors.fill: parent

        handle: Rectangle {
            implicitWidth: 3
            implicitHeight: 4
            color: SplitHandle.pressed ? Material.color(
                                             Material.Grey) //"#81e889"
                                       : (SplitHandle.hovered ? Qt.lighter(
                                                                    Material.color(
                                                                        Material.Grey),
                                                                    1.1) : "555555")
        }
        Rectangle {
            id: contactsField
            implicitWidth: 300
            color: Material.backgroundColor
            Label {
                id: logo_text
                color: Material.color(Material.Red)
                text: "☰ Corp Chat"
                font.family: starsetFont.name
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                //Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                font.pointSize: 28
                //Layout.topMargin: 10
            }

            ContactsView {
                id: contactsView
                //model: contactsModel
                anchors.fill: parent
                anchors.top: logo_text.bottom
                anchors.topMargin: logo_text.height
            }
        }
        Rectangle {
            id: chatField
            implicitWidth: 50
            color: Material.backgroundColor

            ListView {
                id: messagesView
                anchors.fill: parent
                anchors.bottomMargin: messageInput.height + 75
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                spacing: 25 //75

                verticalLayoutDirection: ListView.BottomToTop

                model: messagesModel
                delegate: chooser
            }

            DelegateChooser {
                id: chooser
                role: "type"
                DelegateChoice {
                    roleValue: "text"
                    MessagesDelegate {
                        nickname: model.nickname
                        timeStamp: model.timeStamp
                        message: model.text
                    }
                }
                DelegateChoice {
                    roleValue: "image"
                    ImageDelegate {
                        onImageClick: {
                            dialog.source = source
                            imageClickForm.visible = true
                            animationScaleImageOpen.start()
                            animationOpacityBGOpen.start()
                            //console.log("image should be visible")
                        }
                    }
                }
            }

            MessageInputField {
                id: messageInput
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.bottomMargin: 10
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                onMessageSent: {
                    messagesView.positionViewAtBeginning()
                }
            }
        }
    }
}
