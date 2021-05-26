import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import GlobalQmlSettings 1.0
import QtQuick.Layouts 1.12

import corpchat.models.contactsModel 1.0

ListView
{
    id: list
    focus: true
    clip: true
    model: contactsModel
    highlightFollowsCurrentItem: false
    spacing: 10
    highlight: delegateHighlight
    signal selectedChanged(int idx)
    header: Component {
        RowLayout
        {
            id: rowLayout
            property int btnSize: 45
            width: list.width
            TextField {
                id: newContactInput
                property var accent: Material.color(Material.Teal)
                Material.accent: accent
                Layout.fillWidth: true
                Layout.minimumWidth: rowLayout.btnSize * 3
                Layout.preferredHeight: rowLayout.btnSize
            }
            ToolButton {

                id: newContactButton
                Layout.preferredWidth: rowLayout.btnSize
                Layout.preferredHeight: rowLayout.btnSize
                Layout.alignment: Qt.AlignRight
                contentItem: Image
                {
                    source: "qrc:/qml/icons/add_circle_outline-white-48dp"
                }
                onClicked: {
                    if(newContactInput.text != "") {
                        client.requestContact(newContactInput.text)
                        newContactInput.text = ""
                    }
                }
            }
        }
    }

    Component
    {
        id: delegateHighlight
        Rectangle
        {
            id: highlightImpl
            width: parent.width
            height: 50//contactsDelegate.height + 1
            color: "transparent"
            y: list.currentItem.y
            border.width: 2
            border.color: Material.color(Material.DeepOrange)
            z: 5
            Behavior on y
            {
                PropertyAnimation
                {
                    //easing.type: Easing.bezierCurve
                    duration: 250
                }
            }
        }
    }

    delegate: ContactsDelegate
    {
        id: contactsDelegate
        width: list.width
        //height: 40
        avatarSource: model.avatar
        username: model.nickname
        z: 3
        onClick: {
            list.currentIndex = index
            console.log("selected delegate #" + index)
            selectedChanged(index)

        }

        Connections{
            target: client
            function onNotifyMessage(sender)
            {
                console.log("Notifying on " + sender)
                if(sender === model.nickname)
                    notify()
            }
            Component.onCompleted: {
                client.qmlNotifyUnreadMessage(model.nickname)
            }
        }
    }
    Component.onCompleted:
    {
        selectedChanged.connect(contactsModel.indexChanged)
        //        list.currentIndex = 0
    }

}
