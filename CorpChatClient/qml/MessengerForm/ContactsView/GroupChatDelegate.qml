import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import MessengerForm.RoundImage 1.0
import GlobalQmlSettings 1.0

Rectangle {
    id: delegate

    signal click
    function notify() {
        notification.visible = true
        console.log("Should be visible")
    }

    property string nickname: model.nickname
    property string email: model.email
    property string imgUrl: model.avatar

    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    implicitHeight: 50

    color: Material.background
    Image {
        id: avatar
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 10
        }
        source: "file:///" + imgUrl
        height: 40
        width: 40
    }
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: avatar.right
        anchors.leftMargin: 10
        text: nickname
    }

    MouseArea {
        id: area
        anchors.fill: parent
        z: 5
        onClicked: {
            notification.visible = false
        }
    }
    Component.onCompleted: {
        area.clicked.connect(click)
    }
}
