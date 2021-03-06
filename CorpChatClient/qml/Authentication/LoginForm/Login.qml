import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import GlobalQmlSettings 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import ErrorString 1.0

Rectangle {
    id: root
    //color: Material.backgroundColor
    implicitWidth: GlobalSettings.defaultFormWidth
    implicitHeight: GlobalSettings.defaultFormHeight
    visible: true
    FontLoader {
        id: starsetFont
        source: "../../fonts/FARRAY.otf"
    }
    signal auth(string login, string password)
    signal gotoRegister
    signal baned

    Rectangle {
        id: background
        z: 0
        color: Material.backgroundColor
        anchors.fill: parent
        Label {
            id: logo_text
            color: Material.color(Material.Red)
            text: "Corp Chat"
            font.family: starsetFont.name
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            font.pointSize: 32
            anchors.topMargin: 65
        }
    }

    ColumnLayout {
        id: fields_layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 150
        anchors.bottomMargin: 120

        width: parent.width / 2 + parent.width / 4
        spacing: 2

        TextField {
            id: login_input
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 250
            Layout.fillHeight: false
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignHCenter
            Material.accent: accent
            width: 150
            placeholderText: "login"
            text: "ikozluk160@gmail.com"
            onTextChanged: {
                loginErrorString.visible = false
                loginValidationErrorString.visible = false
                login_input.accent = Material.color(Material.Teal)
            }

            ErrorString {
                id: loginValidationErrorString
                anchors.top: login_input.bottom
                visible: false
                text: qsTr("?????????? ???????????? ??????????????????????")
            }
            ErrorString {
                id: loginErrorString
                anchors.top: login_input.bottom
                visible: false
                text: qsTr("?????????? ????????????????????")
            }
        }
        TextField {
            id: password_input
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 250
            Layout.alignment: Qt.AlignHCenter
            text: "1"
            width: 150
            Material.accent: accent
            placeholderText: "password"

            echoMode: show_password_checkbox.checked ? TextInput.Normal : TextInput.Password

            CheckBox {
                id: show_password_checkbox
                anchors.top: password_input.top
                anchors.left: password_input.right
                anchors.topMargin: 10
                //text: qsTr("???????????????????? ????????????")
                Material.accent: Material.color(Material.DeepOrange)

                indicator: Image {
                    id: checkBoxImage
                    source: show_password_checkbox.checked ? "../../icons/visibility-white.svg" : "../../icons/visibility_off-white.svg"
                }
            }

            ErrorString {
                id: pwErrorString
                anchors.top: password_input.bottom
                visible: false
                text: qsTr("???????????? ????????????????????")
            }

            onTextChanged: {
                pwErrorString.visible = false
                password_input.accent = Material.color(Material.Teal)
            }
        }

        ColumnLayout {
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignHCenter

            spacing: 15

            Button {
                id: confirm_login
                text: qsTr("????????")
                Layout.preferredWidth: parent.width
                Material.accent: Material.color(Material.DeepOrange)
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "#FA8072" : "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                ErrorString {
                    id: authErrorString
                    visible: false
                    text: qsTr("???????????? ??????????.\n?????????????????? ????????????")

                    anchors.bottom: confirm_login.top
                    anchors.bottomMargin: 30
                }
                function checkFields() {
                    function checkField(field, errorHandler) {
                        if (field.text === "") {
                            field.accent = Material.color(Material.Red)
                            errorHandler.visible = true
                            return false
                        }
                        return true
                    }

                    function validateEmail(email) {
                        var re = /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?/
                        if (!re.test(String(email).toLowerCase())) {
                            loginValidationErrorString.visible = true
                            return false
                        }
                        return true
                    }

                    var check_rezult = checkField(
                                login_input,
                                loginErrorString) & checkField(password_input,
                                                               pwErrorString)
                            && validateEmail(login_input.text)

                    return check_rezult
                }
                onClicked: {
                    if (checkFields()) {
                        client.authorize(login_input.text, password_input.text)
                    }
                }
            }
            Button {
                id: start_register
                text: qsTr("??????????????????????")
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "#FA8072" : "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                onClicked: {
                    gotoRegister()
                }
            }
        }
    }
    Connections {
        target: client
        function onAuthFailure() {
            authErrorString.visible = true
        }
    }
    Dialog {
        id: ban_error
        visible: false
        Material.theme: Material.Dark

        Rectangle {
            color: Material.backgroundColor
            implicitWidth: 400
            implicitHeight: 100
            Label {
                anchors.centerIn: parent
                text: "???????????? ???????????????????????? ????????????????????????."
            }
        }

        title: "Dialog"

        standardButtons: StandardButton.Ok

        onButtonClicked: {
            if (clickedButton == StandardButton.Ok) {
                console.log("Accepted " + clickedButton)
                ban_error.visible = false
            }
        }

        Connections {
            Component.onCompleted: console.log("dialog Created!")
            target: client
            onUserBaned: {
                console.log("Dialog is opened")
                ban_error.visible = true
            }
        }
    }
}
