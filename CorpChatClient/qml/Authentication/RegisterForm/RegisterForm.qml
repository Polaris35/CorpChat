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
    implicitWidth: GlobalSettings.defaultFormWidth
    implicitHeight: GlobalSettings.defaultFormHeight
    visible: true
    FontLoader {
        id: starsetFont
        source: "../../fonts/FARRAY.otf"
    }
    signal back

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
        spacing: 1

        TextField {
            id: inputEmail
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.alignment: Qt.AlignHCenter
            width: 150
            Material.accent: accent
            placeholderText: qsTr("Email")
            ErrorString {
                id: inputEmailErrorString
                anchors.top: inputEmail.bottom
                visible: false
                text: qsTr("Введите email")
            }
            ErrorString {
                id: inputEmailValidationErrorString
                anchors.top: inputEmail.bottom
                visible: false
                text: qsTr("email not valid!")
            }
            onTextChanged: {
                inputEmailErrorString.visible = false
                inputEmail.accent = Material.color(Material.Teal)
            }
        }

        TextField {
            id: inputNickname
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.alignment: Qt.AlignHCenter
            width: 150
            Material.accent: accent
            placeholderText: qsTr("Никнейм")
            ErrorString {
                id: nicknameErrorString
                anchors.top: inputNickname.bottom
                visible: false
                text: qsTr("Введите никнейм")
            }
            onTextChanged: {
                nicknameErrorString.visible = false
                inputNickname.accent = Material.color(Material.Teal)
            }
        }

        TextField {
            id: inputPassword
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.alignment: Qt.AlignHCenter
            width: 150
            Material.accent: accent
            placeholderText: qsTr("Пароль")
            echoMode: show_password_checkbox.checked ? TextInput.Normal : TextInput.Password
            CheckBox {
                id: show_password_checkbox
                anchors.top: inputPassword.top
                anchors.left: inputPassword.right
                anchors.topMargin: 10
                //text: qsTr("Показывать пароль")
                Material.accent: Material.color(Material.DeepOrange)

                indicator: Image {
                    id: checkBoxImage
                    source: show_password_checkbox.checked ? "../../icons/visibility-white.svg" : "../../icons/visibility_off-white.svg"
                }
            }
            ErrorString {
                id: pwErrorString
                anchors.top: inputPassword.bottom
                visible: false
                text: qsTr("Пароль обязателен")
            }
            onTextChanged: {
                pwErrorString.visible = false
                inputPassword.accent = Material.color(Material.Teal)
            }
        }
        TextField {
            id: confirmPassword
            property var accent: Material.color(Material.Teal)
            Layout.minimumHeight: 40
            Layout.minimumWidth: 150
            Layout.alignment: Qt.AlignHCenter
            width: 150
            Material.accent: accent
            placeholderText: qsTr("Подтвердите пароль")
            echoMode: TextInput.Password
            ErrorString {
                id: pwcErrorString
                anchors.top: confirmPassword.bottom
                visible: false
                text: confirmPassword.text
                      == inputPassword.text ? qsTr("Подтверждения пароля обязательно") : qsTr(
                                                  "Пароли должны совпадать")
                function checkConditions() {
                    if (confirmPassword.text != inputPassword.text) {
                        pwcErrorString.text = qsTr("Пароли должны совпадать")
                        visible = true
                        return false
                    } else if (confirmPassword.text == "") {
                        pwcErrorString.text = qsTr(
                                    "Подтверждение пароля обязательно")
                        visible = true
                        return false
                    }
                    return true
                }
            }
            onTextChanged: {
                pwcErrorString.visible = false
                confirmPassword.accent = Material.color(Material.Teal)
            }
        }
        Button {
            id: chooseAvatar
            property var imageUrl
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Выберите изображение профиля")
            flat: true
            contentItem: Text {
                text: parent.text
                font: parent.font
                opacity: enabled ? 1.0 : 0.3
                color: parent.down ? "#FA8072" : "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            FileDialog {
                id: chooseAvatarDialog
                title: qsTr("Выберите файл")
                folder: shortcuts.home
                selectMultiple: false
                selectFolder: false
                nameFilters: ["Image files (*.png *.bmp *.jpeg)"]
                onAccepted: {
                    console.log("You chose: " + chooseAvatarDialog.fileUrl)
                    chooseAvatar.imageUrl = chooseAvatarDialog.fileUrl
                    console.log(chooseAvatar.imageUrl)
                    chooseAvatarDialog.close()
                }
                onRejected: {
                    chooseAvatarDialog.close()
                }
            }
            onClicked: {
                chooseAvatarDialog.visible = true
            }
        }
        ErrorString {
            id: chooseAvatarErrorString
            visible: false
            text: qsTr("Ошибка аватар не выбран")
            anchors.bottom: registerErrorString.top
            anchors.bottomMargin: 30
        }
        ErrorString {
            id: registerErrorString
            visible: false
            text: qsTr("Ошибка Регистрации.\nВероятно, данный email уже зарегестрирован")
            anchors.bottom: confirmRegistration.top
            anchors.bottomMargin: 30
        }
        Button {
            id: confirmRegistration
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Регистрация")
            contentItem: Text {
                text: parent.text
                font: parent.font
                opacity: enabled ? 1.0 : 0.3
                color: parent.down ? "#FA8072" : "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
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
                        inputEmailValidationErrorString.visible = true
                        return false
                    }
                    return true
                }

                var check_rezult = checkField(inputPassword, pwErrorString)
                        & checkField(confirmPassword,
                                     pwcErrorString) & checkField(
                            inputNickname, nicknameErrorString)
                        & checkField(chooseAvatar.imageUrl,
                                     chooseAvatarErrorString) & checkField(
                            inputEmail, inputEmailErrorString)
                        && validateEmail(inputEmail.text)
                return check_rezult
            }

            onClicked: {
                if (checkFields() && pwcErrorString.checkConditions())
                    client.registerNewUser(inputNickname.text, inputEmail.text,
                                           inputPassword.text,
                                           chooseAvatar.imageUrl)
            }
        }
        Button {
            id: backButton
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Назад")
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
                back()
            }
        }
    }
    Connections {
        target: client
        function onRegisterSuccess() {
            back()
        }
        function onRegisterFailure() {
            registerErrorString.visible = true
        }
    }
}
