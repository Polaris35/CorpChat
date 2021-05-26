import QtQuick 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0
import GlobalQmlSettings 1.0

Image
{
    id: root
    property int size
    property bool rounded: true
    property bool adapt: true
    width: size
    height: size
    //clip: true
    fillMode: Image.PreserveAspectFit
    layer.enabled: rounded
        layer.effect: OpacityMask {
            maskSource: Item {
                width: root.width
                height: root.height
                Rectangle {
                    anchors.centerIn: parent
                    width: root.adapt ? root.width : Math.min(root.width, root.height)
                    height: root.adapt ? root.height : width
                    radius: Math.min(width, height)
                }
            }
        }
}
