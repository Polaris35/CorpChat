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
    property alias url: messageImage.source
    property bool sentByMe: model.nickname !== contactsModel.currentDialog()
    ListView.onAdd: {
        messagesView.positionViewAtBeginning()
    }
    anchors.right: sentByMe ? parent.right : undefined
    spacing: 6
    RowLayout {
        id: messageRow
        spacing: 6
        anchors.right: sentByMe ? parent.right : undefined

        Image {
            id: avatar
            source: !sentByMe ? Qt.resolvedUrl(
                                    "file:///" + contactsModel.currentAvatar(
                                        )) : ""
            Layout.preferredHeight: 40
            Layout.preferredWidth: 40
            anchors.right: messageImage.left
            anchors.bottom: messageImage.bottom
        }

        Image {
            id: messageImage
            asynchronous: true
            source: Qt.resolvedUrl("file:///" + model.text)
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200
            Component.onCompleted: {
                console.log(Qt.resolvedUrl("file:///" + model.text))
                console.log(messageImage.status.toString())
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
