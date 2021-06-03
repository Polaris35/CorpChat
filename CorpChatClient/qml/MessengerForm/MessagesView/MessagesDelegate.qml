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
    }
}
