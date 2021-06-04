import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import Qt.labs.qmlmodels 1.0
//import GlobalQmlSettings 1.0
import MessengerForm.RoundImage 1.0

Column {
    width: ListView.view.width
    property string url
    readonly property bool sentByMe: model.nickname !== contactsModel.currentDialog()
    ListView.onAdd: {
        messagesView.positionViewAtBeginning()
    }
    anchors.right: sentByMe ? parent.right : undefined
    spacing: 6
    RowLayout {
        id: messageRow
        anchors.right: sentByMe ? parent.right : undefined
        spacing: 6
        Image {
            id: avatar
            source: !sentByMe ? Qt.resolvedUrl(
                                    "file:///" + contactsModel.currentAvatar(
                                        )) : ""
            Layout.preferredHeight: 40
            Layout.preferredWidth: 40
            //            anchors.right: messageDocument.left
            //            anchors.bottom: messageDocument.bottom
            //            anchors.rightMargin: 6
        }

        Rectangle {
            id: messageDocument
            color: sentByMe ? "lightgrey" : "steelblue"
            Layout.preferredHeight: 60
            Layout.preferredWidth: 250
            radius: 5
            Row {
                id: row
                spacing: 5
                Image {
                    id: fileImage
                    source: "qrc:/qml/icons/documents_icon.svg"
                    width: 50
                    height: 50
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: 5
                    anchors.leftMargin: 5
                    Component.onCompleted: {
                        console.log(url.toString())
                        fileName.text = url.replace(/^.*[\\\/]/,
                                                    '').split("+", 2)[1]
                        console.log(fileName.text)
                    }
                }

                Text {
                    id: fileName
                    width: messageDocument - 5 - 50 - 20
                    text: "sample textaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.txt"
                    z: 2
                    color: sentByMe ? "black" : "white"
                    font.pixelSize: 14
                    //                    verticalAlignment: Text.AlignVCenter
                    //                    horizontalAlignment: TextEdit.AlignHCenter
                    wrapMode: Text.Wrap
                    anchors.top: parent.top
                    anchors.left: fileImage.right
                    anchors.leftMargin: 20
                    anchors.topMargin: 15
                    //                    Layout.top: 10
                    //                    Layout.left: 10
                    //anchors.centerIn: parent
                }
            }
            MouseArea {
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.leftMargin: 0
                onClicked: {
                    Qt.openUrlExternally(url)
                }
            }
        }
    }
    Label {
        id: timestampText
        text: /*Qt.formatDateTime(*/ model.timeStamp /*, "d MMM hh:mm")*/
        color: "black"
        anchors.right: sentByMe ? parent.right : undefined
    }
}
