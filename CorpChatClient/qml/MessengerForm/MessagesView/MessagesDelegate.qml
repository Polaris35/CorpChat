import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import Qt.labs.qmlmodels 1.0
import GlobalQmlSettings 1.0
import MessengerForm.RoundImage 1.0

Column {
    width: ListView.view.width    
    property string nickname
    property string timeStamp
    property alias message: messageText.text
    readonly property bool sentByMe: nickname !== contactsModel.currentDialog()

    //"qrc:/qml/Images/Default"//model.avatar
    anchors.right: nickname !== contactsModel.currentDialog(
                       ) ? messagesView.contentItem.right : undefined
    spacing: 6

    Row {
        id: messageRow
        spacing: 6
        anchors.right: nickname !== contactsModel.currentDialog(
                           ) ? parent.right : undefined

        Image {
            id: avatar
            source: !(nickname !== contactsModel.currentDialog(
                          )) ? Qt.resolvedUrl(
                                   "file:///" + contactsModel.currentAvatar(
                                       )) : ""
            width: 40
            height: 40
        }

        Rectangle {
            width: Math.min(
                       messageText.implicitWidth + 24, messagesView.width
                       - (!(nickname !== contactsModel.currentDialog(
                                )) ? avatar.width + messageRow.spacing : 0))
            height: messageText.implicitHeight + 24
            color: nickname !== contactsModel.currentDialog(
                       ) ? "lightgrey" : "steelblue"

            Label {
                id: messageText
                //text: model.text
                color: nickname !== contactsModel.currentDialog(
                           ) ? "black" : "white"
                anchors.fill: parent
                anchors.margins: 12
                wrapMode: Label.Wrap
            }
        }
    }

    Label {
        id: timestampText
        text: /*Qt.formatDateTime(*/ timeStamp /*, "d MMM hh:mm")*/
        color: "black"
        anchors.right: nickname !== contactsModel.currentDialog(
                           ) ? parent.right : undefined
    }

    ListView.onAdd: {
        messagesView.positionViewAtBeginning()
        //                        console.log(Qt.formatDateTime(model.timeStamp,
        //                                                      "d MMM hh:mm"))
        //                        console.log(model.timeStamp)
    }
}//Item {
//    id: name

//    property string avatarSource
//    property string username
//    property string time
//    property string message
//    property string type
//    property var delegateWidth

//    Column {
//        anchors.right: sentByMe ? listView.contentItem.right : undefined
//        spacing: 6

//        readonly property bool sentByMe: username !== client.username()

//        Row {
//            id: messageRow
//            spacing: 6
//            anchors.right: sentByMe ? parent.right : undefined

//            Image {
//                id: avatar
//                source: avatarSource
//            }

//            Rectangle {
//                width: Math.min(
//                           messageText.implicitWidth + 24,
//                           listView.width - (!sentByMe ? avatar.width + messageRow.spacing : 0))
//                height: messageText.implicitHeight + 24
//                color: sentByMe ? "lightgrey" : "steelblue"

//                Label {
//                    id: messageText
//                    text: message
//                    color: sentByMe ? "black" : "white"
//                    anchors.fill: parent
//                    anchors.margins: 12
//                    wrapMode: Label.Wrap
//                }
//            }
//        }

//        Label {
//            id: timestampText
//            text: Qt.formatDateTime(time, "d MMM hh:mm")
//            color: "lightgrey"
//            anchors.right: sentByMe ? parent.right : undefined
//        }
//    }
//}
//DelegateChooser {//    id: root
//    role: "type"
//    property string avatarSource
//    property string username
//    property string time
//    property string message
//    property string type
//    property var delegateWidth

//    DelegateChoice // text delegate
//    {
//        // begin
//        roleValue: "text"
//        RowLayout {
//            id: rowLayout

//            spacing: 10
//            RoundImage {
//                id: icon
//                source: avatarSource
//                Layout.preferredWidth: 40
//                Layout.preferredHeight: 40
//                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//            }

//            ColumnLayout {
//                id: layer
//                width: delegateWidth
//                Item {
//                    id: messageContent

//                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//                    Layout.leftMargin: 5
//                    Layout.fillWidth: true
//                    RowLayout {
//                        Label {
//                            id: senderNickname

//                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//                            text: username
//                        }
//                        Label {
//                            id: timeSpan

//                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//                            text: time
//                        }
//                    }
//                }

//                Label {
//                    id: messageText
//                    text: message
//                    visible: true
//                    wrapMode: Text.WordWrap
//                    Layout.preferredHeight: contentHeight
//                    Layout.preferredWidth: rowLayout.width - icon.width
//                    Layout.topMargin: senderNickname.contentHeight
//                }
//            }
//        }
//    } // end
//    DelegateChoice //image delegate
//    {
//        roleValue: "image"
//        //begin
//        RowLayout {
//            id: rowLayout1
//            width: delegateWidth
//            spacing: 10
//            RoundImage {
//                id: icon1
//                source: avatarSource
//                Layout.preferredWidth: 40
//                Layout.preferredHeight: 40
//                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//            }

//            ColumnLayout {
//                id: layer1
//                Item {
//                    id: messageContent1

//                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
//                    Layout.leftMargin: 5
//                    Layout.fillWidth: true
//                    RowLayout {
//                        Label {
//                            id: senderNickname1

//                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//                            text: username
//                        }
//                        Label {
//                            id: timeSpan1

//                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//                            text: time
//                        }
//                    }
//                }

//                Image {
//                    id: pictures
//                    source: message
//                    visible: true
//                    Layout.preferredHeight: contentHeight
//                    Layout.preferredWidth: rowLayout1.width - icon1.width
//                    Layout.topMargin: senderNickname1.contentHeight
//                }
//            }
//        }
//    } //end
//}

