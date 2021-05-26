pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property int    defaultFormWidth: 1280
    readonly property int    defaultFormHeight: 720
    readonly property int    loginFormWidth: 540
    readonly property int    loginFormHeight: 680
    //properties, get from C++ singleton object
    readonly property string username: "Raiden"
    //readonly property string starsetFont: starsetFont.name
    //readonly property alias starsetFont: starsetFont
}
