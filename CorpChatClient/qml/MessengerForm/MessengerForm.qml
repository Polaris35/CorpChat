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
    function validateEmail(email) {
        var re = /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?/
        return re.test(String(email).toLowerCase())
    }
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
                text: "Corp Chat"
                color: Material.color(Material.Red)
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
                onNewDialog: {
                    //dialog.source = source
                    newDialogClickForm.visible = true
                    animationScaleDialogOpen.start()
                    animationOpacityBGDialogOpen.start()
                }
                onNewConversation: {
                    newConversationClickForm.visible = true
                    animationScaleConversationOpen.start()
                    animationOpacityBGConversationOpen.start()
                }
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
                DelegateChoice {
                    roleValue: "document"
                    DocumentDelegate {
                        url: model.text
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
    // new Dialog begin
    Rectangle {
        id: newDialogClickForm
        visible: false
        opacity: 1
        color: Qt.rgba(0, 0, 0, 0.75)
        anchors.fill: parent
        anchors.centerIn: parent
        z: 2
        layer.enabled: true

        Rectangle {
            id: newDialog
            z: 3
            opacity: 1
            width: 500
            height: 195
            anchors.centerIn: parent
            color: Material.backgroundColor

            Label {
                id: input_email_lbl
                anchors.horizontalCenter: parent.horizontalCenter
                anchors {
                    top: parent.top
                    topMargin: 10
                }

                text: qsTr("input email")
                minimumPixelSize: 18
                horizontalAlignment: TextEdit.AlignHCenter
                color: Material.color(Material.Red)
                font.family: starsetFont.name
                font.pixelSize: 26
            }
            TextField {
                id: email
                anchors {
                    top: input_email_lbl.bottom
                    topMargin: 10
                    left: parent.left
                    leftMargin: 100
                }

                width: 300
                placeholderText: qsTr("email")
                horizontalAlignment: TextEdit.AlignHCenter
            }

            Rectangle {
                anchors {
                    top: email.bottom
                    topMargin: 10
                }
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: cancel
                    anchors {
                        left: parent.left
                        leftMargin: 75
                    }

                    //width: parent.width / 2 - 4
                    text: qsTr("cancel")
                    implicitWidth: 150
                    onClicked: {
                        animationOpacityBGDialogClose.start()
                        animationScaleDialogClose.start()
                    }
                }
                Button {
                    id: submit
                    //width: parent.width - 4
                    anchors {
                        right: parent.right
                        rightMargin: 75
                    }

                    text: qsTr("submit")
                    implicitWidth: 150
                    onClicked: {
                        if (email.text !== "") {
                            client.requestContact(email.text)
                            email.text = ""
                            animationOpacityBGDialogClose.start()
                            animationScaleDialogClose.start()
                        }
                    }
                }
            }
        }

        ColorAnimation {
            id: animationOpacityBGDialogOpen
            target: newDialogClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.3)
            to: Qt.rgba(0, 0, 0, 0.75)
            duration: 100
        }

        ColorAnimation {
            id: animationOpacityBGDialogClose
            target: newDialogClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.75)
            to: Qt.rgba(0, 0, 0, 0.3)
            duration: 99
        }

        ScaleAnimator {
            id: animationScaleDialogOpen
            target: newDialog
            from: 0.4
            to: 1
            duration: 100
        }

        ScaleAnimator {
            id: animationScaleDialogClose
            target: newDialog
            from: 1
            to: 0.4
            duration: 100
            onFinished: {
                newDialogClickForm.visible = false
            }
        }
    } // end new Dialog

    //newConversation begin
    Rectangle {
        id: newConversationClickForm
        visible: false
        opacity: 1
        color: Qt.rgba(0, 0, 0, 0.75)
        anchors.fill: parent
        z: 2
        layer.enabled: true

        Rectangle {
            id: newConversation
            z: 3
            opacity: 1
            width: 500
            height: 700
            anchors.centerIn: parent
            color: Material.backgroundColor
            Layout.maximumHeight: 700

            StackView {
                id: newConversationStackView
                anchors.fill: parent
                initialItem: newConversationInitialItemComponent
                Component {
                    id: searchOtherUser
                    Rectangle {
                        color: "transparent"
                        anchors.fill: parent
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
                            placeholderText: "nickname or email"
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
                                    if (validateEmail(searchField.text))
                                        console.log("123")
                                    else
                                        console.log("456")
                                } else
                                    searchField.placeholderTextColor = Material.color(
                                                Material.Red)
                            }
                        }

                        Rectangle {
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
                                    newConversationStackView.pop()
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
                                    newConversationStackView.pop()
                                }
                            }
                        }
                    }
                }
                Component {
                    id: newConversationInitialItemComponent
                    Rectangle {
                        id: newConversationInitialItem
                        z: 4
                        anchors.fill: parent
                        color: Material.background
                        Rectangle {
                            id: newConversationButtonGroup
                            implicitHeight: 50
                            color: "transparent"
                            anchors {
                                bottom: parent.bottom
                            }
                            width: parent.width

                            Button {
                                id: conversationCancel
                                anchors {
                                    left: parent.left
                                    leftMargin: 10
                                }

                                //width: parent.width / 2 - 4
                                text: qsTr("cancel")
                                implicitWidth: 150
                                onClicked: {
                                    animationOpacityBGConversationClose.start()
                                    animationScaleConversationClose.start()
                                }
                            }
                            Button {
                                id: otherUser
                                //width: parent.width - 4
                                anchors {
                                    right: conversationSubmit.left
                                    rightMargin: 10
                                    left: conversationCancel.right
                                    leftMargin: 10
                                }

                                text: qsTr("add Other user")
                                implicitWidth: 150
                                onClicked: {
                                    if (newConversationStackView.depth > 1)
                                        newConversationStackView.pop()
                                    newConversationStackView.push(
                                                searchOtherUser)
                                }
                            }
                            Button {
                                id: conversationSubmit
                                //width: parent.width - 4
                                anchors {
                                    right: parent.right
                                    rightMargin: 10
                                }

                                text: qsTr("submit")
                                implicitWidth: 150
                                onClicked: {
                                    animationOpacityBGDialogClose.start()
                                    animationScaleDialogClose.start()
                                }
                            }
                        }
                    }
                }
            }
        }

        ColorAnimation {
            id: animationOpacityBGConversationOpen
            target: newConversationClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.3)
            to: Qt.rgba(0, 0, 0, 0.75)
            duration: 100
        }

        ColorAnimation {
            id: animationOpacityBGConversationClose
            target: newConversationClickForm
            property: "color"
            from: Qt.rgba(0, 0, 0, 0.75)
            to: Qt.rgba(0, 0, 0, 0.3)
            duration: 99
        }

        ScaleAnimator {
            id: animationScaleConversationOpen
            target: newConversation
            from: 0.4
            to: 1
            duration: 100
        }

        ScaleAnimator {
            id: animationScaleConversationClose
            target: newConversation
            from: 1
            to: 0.4
            duration: 100
            onFinished: {
                newConversationClickForm.visible = false
            }
        }
    } // end new Conversation
}
