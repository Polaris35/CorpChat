import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Item {
    id: root
    property alias spacing: _row.spacing
    property alias text: _string.text
    Row
    {
        id: _row
        spacing: 7
        Image
        {
            source: "../icons/warning-red.svg"
        }

        Label
        {
            id: _string
            color: Material.color(Material.Red)
        }
    }
}
