import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import MessengerForm.RoundImage 1.0
import GlobalQmlSettings 1.0

//Rectangle {
//    id: root
//    property string avatarSource
//    property string username

//    height: 50
//    signal click
//    function notify() {
//        notification.visible = true
//        console.log("Should be visible")
//    }

//    color: Material.backgroundColor
//    Rectangle {
//        id: delimiter
//        height: 1
//        width: rowLayout.width
//        color: Qt.lighter(Material.backgroundColor)
//        anchors.top: rowLayout.bottom
//        //        anchors.bottomMargin: 10
//    }
//    RowLayout {
//        id: rowLayout
//        anchors.fill: parent
//        Image {
//            id: avatar
//            source: "file:///" + avatarSource
//            Layout.preferredWidth: 40
//            Layout.preferredHeight: 40
//            Layout.leftMargin: 5
//            Layout.alignment: Qt.AlignLeft
////            Component.onCompleted: {
////                console.log("Avatar source = " + avatarSource)
////            }
//        }
//        Label {
//            id: usernameLabel
//            text: username
//            Layout.preferredHeight: contentHeight
//            //            Layout.preferredWidth: rowLayout.width - avatar.width
//            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//        }
//        Rectangle {
//            id: notification
//            color: Material.color(Material.Orange)
//            radius: 10
//            visible: false
//            Layout.preferredHeight: 10
//            Layout.preferredWidth: 10
//            Layout.rightMargin: 20
//            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
//        }
//    }
//    MouseArea {
//        id: area
//        anchors.fill: parent
//        z: 5
//        onClicked: {
//            notification.visible = false
//        }
//    }
//    Component.onCompleted: {
//        area.clicked.connect(click)
//    }
//}
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
