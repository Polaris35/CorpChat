/*
 * InconClickHighlight.qml
 * Компонент, отвечающий за подсвечивание иконок при нажатии.
 */

import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

Rectangle
{
    id: highlight
    property int maxRadius
    function run()
    {
        highlight.running = true
    }
    property bool running
    width: 0
    height: 0
    anchors.centerIn: parent
    opacity: 0.0
    color: "white"
    radius: Math.min(width, height)
    SequentialAnimation
    {
        id: highlightAnim
        running: highlight.running
        ParallelAnimation
        {
            PropertyAnimation {
                target: highlight
                property: "opacity"
                from: 0
                to: 0.5
                duration: 200
            }
            PropertyAnimation
            {
                target: highlight
                property: "width"
                from: 0
                to: highlight.maxRadius
                easing.type: Easing.InOutQuad
                duration: 250
            }
            PropertyAnimation
            {
                target: highlight
                property: "height"
                from: 0
                to: highlight.maxRadius
                easing.type: Easing.InOutQuad
                duration: 250
            }
        }

            PropertyAnimation {
                target: highlight
                property: "opacity"
                from: 0.5
                to: 0
                duration: 200
            }

    }
}
